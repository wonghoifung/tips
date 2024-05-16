import struct
import asyncio
from hashlib import sha1
import datetime
import sys

from .tracker import Tracker
from .torrent import Torrent
from .peer import Peer
from .file_manager import FileManager
from .pieces import Pieces
from .exception import CustomError

KEEPALIVE = bytes([0, 0, 0, 0])
CHOKE = bytes([0, 0, 0, 1]) + bytes([0])
UNCHOKE = bytes([0, 0, 0, 1]) + bytes([1])
INTERESTED = bytes([0, 0, 0, 1]) + bytes([2])
NOT_INTERESTED = bytes([0, 0, 0, 1]) + bytes([3])

def is_udp_tracker(url):
    return url.startswith('udp')

class TorrentClient():

    def __init__(self, torrent_file, download_destination, loop):
        self.loop = loop
        self.torrent = Torrent(torrent_file)
        self.pieces = Pieces(self.torrent)
        self.pieces_downloaded = []
        tracker = Tracker(self.torrent)
        peers_list = tracker.connect(True)
        print('peers count:{},{}'.format(len(peers_list),peers_list))
        if len(peers_list) == 0:
            sys.exit()
        self.failed_peer_count = 0
        self.peers = [Peer(p['hostname'], p['port'], self.torrent)
                for p in peers_list]
        self.active_peers = []
        self.file_manager = FileManager(self.torrent, download_destination)

    def _hand_shake(self):
        pstrlen = 19
        pstr = 'BitTorrent protocol'
        reserved = bytes(8)
        # return a long byte string
        return b''.join([
            bytes([pstrlen]),
            bytes(pstr, encoding='utf-8'),
            reserved,
            self.torrent.info_hash,
            bytes(self.torrent.peer_id, encoding='utf-8')
        ])

    async def connect_to_peers(self):
        await asyncio.gather(
                *[self._connect_to_peer(peer) for peer in self.peers],
                )

    async def keep_alive(self):
        await asyncio.gather(
                *[self._send_keepalive_to_peer(peer) for peer in self.peers],
                )

    async def _send_keepalive_to_peer(self, peer):
        while True:
            await asyncio.sleep(90)
            if peer in self.active_peers:
                if not peer.writer.transport.is_closing():
                    try:
                        peer.writer.write(KEEPALIVE)
                        await peer.writer.drain()
                        print('just sent keepalive message to {}'.format(peer.address))
                    except Exception as e:
                        print('keepalive error: {}'.format(e))
                else:
                    print('connection closed by {}'.format(peer.address))
                    self._close_connection(peer)
                    if len(self.active_peers) == 0:
                        print('no active peers left, stop loop')
                        self.loop.stop()

    def _close_connection(self, peer):
        self.active_peers.remove(peer)
        peer.writer.close()

    async def _connect_to_peer(self, peer):
        try:
            reader, writer = await asyncio.open_connection(
                    peer.address['host'], peer.address['port'])

            peer.reader = reader
            peer.writer = writer

            handshake_ok = await self._connection_handler(peer)
            if handshake_ok:
                await self._receive_data(peer)
        except (ConnectionRefusedError,
                ConnectionResetError,
                ConnectionAbortedError,
                TimeoutError, OSError) as e:
            print('connect to peer error: {}, {}'.format(e, peer.address))
            self.failed_peer_count += 1

        if self.failed_peer_count == len(self.peers):
            print('all peers connect failed, stop loop')
            # raise CustomError('all peers connect failed',100)
            self.loop.stop()

    async def _connection_handler(self, peer):
        print('connected with peer {}'.format(peer.address))
        self.active_peers.append(peer)
        self.timer = datetime.datetime.now()
        try:
            peer.writer.write(self._hand_shake())
            await peer.writer.drain()
        except (IOError, Exception) as e:
            print('handshake error: {}'.format(e))
            return False

        hand_shake_msg = b''
        while True:
            try:
                chunk = await peer.reader.read(68)
                hand_shake_msg += chunk
                if not chunk:
                    print('read nothing, connection closed: {}'.format(peer.address))
                    self._close_connection(peer)
                    return False
                # 68 is the length of handshake message
                if len(hand_shake_msg) < 68:
                    continue
                info_hash = hand_shake_msg[28:48]
                if self.torrent.info_hash != info_hash:
                    print('handshake info_hash not matched, connection refused {}'.format(peer.address))
                    self._close_connection(peer)
                    return False
                else:
                    peer.writer.write(INTERESTED)
                    await peer.writer.drain()
                    print('Sent INTERESTED message to Peer {}'.format(peer.address))
                    return True
            except (ConnectionRefusedError, TimeoutError, Exception) as e:
                print('peer read-HANDSHAKE/write-INTERESTED error: {}'.format(e))
                self._close_connection(peer)
                return False

    async def _receive_data(self, peer):
        while True:
            message_body = b''
            first_4_bytes = b''
            while len(first_4_bytes) < 4:
                try:
                    chunk = await peer.reader.read(4 - len(first_4_bytes))
                except Exception as e:
                    print('read from peer {}, Exception: {}'.format(peer.address, e))
                    return

                if not chunk:
                    print('read nothing. connection closed. {}'.format(peer.address))
                    return
                first_4_bytes += chunk

            print('first 4 bytes: {}'.format(first_4_bytes))
            message_length = struct.unpack('!i', first_4_bytes)[0]

            if message_length == 0:
                peer.timer = datetime.datetime.now()
                print('Peer {} sent KEEPALIVE message'.format(peer.address))
            else:
                while len(message_body) < message_length:
                    try:
                        message_body_chunk = await peer.reader.read(message_length - len(message_body))
                    except Exception as e:
                        print('read message body from peer {}, error: {}'.format(peer.address, e))
                        return
                    if not message_body_chunk:
                        print('read nothing. connection closed. {}'.format(peer.address))
                        return
                    message_body += message_body_chunk
                if message_body[0] == 7:
                    print('message piece received')
                else:
                    print('message body: {}'.format(message_body))

                message_id = message_body[0]
                payload = message_body[1:]
                await self._message_handler(peer, message_id, payload)

    async def _message_handler(self, peer, msg_id, payload):
        """ identity type of message sent from peer and make action accordingly """

        if msg_id == 0:
            print('Peer {} sent CHOKE message'.format(peer.address))

        elif msg_id == 1:
            print('Peer {} sent UNCHOKE message'.format(peer.address))
            peer.choked = False
            await self._request_piece_one_block(peer)

        elif msg_id == 2:
            print('Peer {} sent INTERESTED message'.format(peer.address))

        elif msg_id == 3:
            print('Peer {} sent NOT INTERESTED message'.format(peer.address))

        elif msg_id == 4:
            # peer tells what other pieces it has
            # we need to update our record
            print('Peer {} sent HAVE message'.format(peer.address))
            index = struct.unpack('!i', payload)[0]
            print('{} has piece {}'.format(peer.address['host'], index))
            no_pieces = len(peer.queue) == 0
            peer.queue.add(index)
            if no_pieces:
                await self._request_piece_one_block(peer)

        elif msg_id == 5:
            # message payload is what pieces the peer has, labeled by indexes
            # we need to keep a record of what the peer has
            print('Peer {} sent BITFIELD message'.format(peer.address))
            no_pieces = len(peer.queue) == 0
            b = bytearray(payload)
            bitstring = ''.join([bin(x)[2:] for x in b])
            pieces_indexes = [i for i, x in enumerate(bitstring) if x == '1']
            for index in pieces_indexes:
                peer.queue.add(index)

            print('{} has {}'.format(peer.address['host'], pieces_indexes))
            if no_pieces:
                await self._request_piece_one_block(peer)

        elif msg_id == 6:
            print('Peer {} sent REQUEST message'.format(peer.address))

        elif msg_id == 7:
            print('Peer {} sent PIECE message'.format(peer.address))
            await self._handle_piece_msg(payload, peer)

        elif msg_id == 8:
            print('Peer {} sent CANCEL message'.format(peer.address))

    def _request_message(self, block):
        message_id = b'\x06'
        message_length = bytes([0, 0, 0, 13])

        msg = b''.join([
            message_length,
            message_id,
            struct.pack('!I', block['index']),
            struct.pack('!I', block['begin_offset']),
            struct.pack('!I', block['request_length'])
            ])

        return msg

    async def _request_piece_one_block(self, peer):

        if not peer.choked:
            while len(peer.queue) > 0:
                index_left = set()
                for b in peer.queue.queue:
                    index_left.add(b['index'])
                print('peer queue left {}'.format(index_left))
                print('{} blocks left to request from {}'.format(len(peer.queue.queue), peer.address['host']))
                block = peer.queue.pop()
                if self.pieces.needed(block):
                    try:
                        peer.writer.write(self._request_message(block))
                        await peer.writer.drain()
                    except Exception as e:
                        print('write REQUEST error: {}'.format(e))
                    print('requested {} from {}'.format(block, peer.address['host']))
                    self.pieces.add_requested(block)
                    break

    async def _handle_piece_msg(self, message_payload, peer):
        if self.file_manager.is_complete():
            print('complete! ignore piece from {}!'.format(peer.address))
            return

        index = struct.unpack('!i', message_payload[:4])[0]
        begin_offset = struct.unpack('!i', message_payload[4:8])[0]
        payload = message_payload[8:]
        block = {
            'index': index,
            'begin_offset': begin_offset,
            'request_length': len(payload),
            'payload': payload
        }

        print('got a block from {}, {}, {}'.format(peer.address, block['index'], block['begin_offset']))
        piece_index, piece = self.pieces.add_received(block)

        if piece is not None:
            hashed_piece = sha1(piece).digest()
            # print(self.torrent.piece_hash_list[piece_index])
            # print(hashed_piece)
            if self.torrent.piece_hash_list[piece_index] == hashed_piece:
                self.pieces_downloaded.append(piece_index)

                # print('we have piece {}'.format(piece_index))
                # print('downloaded: {}, total: {}'.format(len(self.pieces_downloaded), self.torrent.number_of_pieces))
                try:
                    self.file_manager.write(piece_index, piece)
                except IOError as e:
                    print('file mangager write error:{}'.format(e))
                if len(self.pieces_downloaded) == self.torrent.number_of_pieces:
                    print('finished downloading!!!')
                    return
                print('percentage {:.2f}%'.format((len(self.pieces_downloaded) * 100) / self.torrent.number_of_pieces))
            else:
                self.pieces.discard_piece(piece_index)
                for p in self.active_peers:
                    p.queue.add(piece_index)

        await self._request_piece_one_block(peer)
