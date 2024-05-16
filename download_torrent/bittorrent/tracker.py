import socket
import struct
import time
import urllib
from random import randint
from urllib.parse import urlparse, urlencode
from urllib.request import urlopen
import bencodepy
# from bcoding import bdecode

CONNECT = 0
ANNOUNCE = 1
DEFAULT_CONNECTION_ID = 0x41727101980

class Tracker():

    def __init__(self, torrent):
        self.torrent = torrent
        # self.url = self.torrent.announce_url.decode('utf-8')
        self.url_list = self.torrent.announce_list
        # u = urlparse(self.url)
        # self.scheme = u.scheme
        # self.hostname = u.hostname
        # self.port = u.port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.timeout = 2
        self.connection_id = DEFAULT_CONNECTION_ID

    def connect(self, getall):
        peerlst = []
        for url in self.url_list:
            print('get peers from tracker: {}'.format(url))
            u = urlparse(url[0].decode('utf-8'))
            self.url = url[0].decode('utf-8')
            self.scheme = u.scheme
            self.hostname = u.hostname
            self.port = u.port
            if self.scheme == 'udp':
                lst = self._connect_via_udp()
                if lst is not None and len(lst) > 0:
                    if getall:
                        peerlst.extend(lst)
                    else:
                        return lst
            elif self.scheme == 'http' or self.scheme == 'https':
                lst = self._connect_via_http()
                if lst is not None and len(lst) > 0:
                    if getall:
                        peerlst.extend(lst)
                    else:
                        return lst
            else:
                print('only support udp/http/https, tracker url: {}'.format(url))
        return peerlst

    def _connect_via_udp(self):
        connect_response = self._connect_request()
        if connect_response is None:
            print('udp tracker no CONNECT response: {}'.format(self.url))
            return []
        action=-1
        try:
            action, transition_id, connection_id = struct.unpack('!LLQ', connect_response)
            self.connection_id = connection_id
            self.transition_id = transition_id
        except struct.error as e:
            print('udp tracker CONNECT response err: {}, res:{}'.format(e,connect_response))
            return []
        if action == CONNECT:
            announce_response = self._announce_request(transition_id)
            if announce_response is None:
                print('udp tracker no ANNOUNCE response: {}'.format(self.url))
                return []
            action, transition_id, interval = struct.unpack('!3I', announce_response[:12])
            if transition_id == self.transition_id:
                bin_peers = announce_response[20:]
                return self._decode_peers(bin_peers)
        else:
            print('udp tracker CONNECT response action not matched: {}, res:{}'.format(e,connect_response))
            return []

    def _announce_request(self, transition_id):
        message = b''.join([
            struct.pack('!Q', self.connection_id),
            struct.pack('!I',  ANNOUNCE),
            struct.pack('!I', transition_id),
            struct.pack('!20s', self.torrent.info_hash),
            struct.pack('!20s', str.encode(self.torrent.peer_id)),
            struct.pack('!Q', 0),
            struct.pack('!Q', self.torrent.left),
            struct.pack('!Q', 0),
            struct.pack('!I', 2),
            struct.pack('!I', 0),
            struct.pack('!i', -1),
            struct.pack('!I', randint(0, 2**32 -1)),
            struct.pack('!H', 6881)
        ])
        return self._send_message_and_wait_response(message)

    def _connect_request(self):
        action = CONNECT
        transition_id = randint(0, 2**32 -1)
        message = struct.pack('!QLL', self.connection_id, action, transition_id)
        return self._send_message_and_wait_response(message)

    def _send_message_and_wait_response(self, message):
        try:
            self.sock.sendto(message, (self.hostname, self.port))
            self.sock.settimeout(self.timeout)
            response = self.sock.recv(1024)
            return response
        except socket.timeout:
            print('wait udp tracker response timeout, {}'.format(self.url))
            return None
        except socket.gaierror as e:
            print('udp tracker err: {}, {}'.format(e, self.url))
            return None

    def _connect_via_http(self):
        params = {
            'info_hash': self.torrent.info_hash,
            'peer_id': self.torrent.peer_id,
            'left': self.torrent.left,
            'downloaded': 0,
            'uploaded': 0,
            'port': 6881,
            'compact': 1,
            'event': 'started'
        }
        try:
            url = self.url + '?' + urlencode(params)
            r = urlopen(url, timeout=5)
            # response = bdecode(r.read())
            response = bencodepy.decode(r.read())
            if 'failure reason' not in response:
                return self._decode_peers(response[b'peers'])
            else:
                print('http tracker failure: {}'.format(response))
                return []
                # time.sleep(response[b'interval'])
        except urllib.error.HTTPError as e:
            print('http tracker http error:{}, {}'.format(e, self.url))
        except urllib.error.URLError as e:
            print('http tracker url error:{}, {}'.format(e, self.url))
        return []

    def _decode_peers(self, bin_peers):
        """
        The first 4 bytes contain the 32-bit ipv4 address.
        The remaining two bytes contain the port number.
        Both address and port use network-byte order.
        """
        offset = 0
        peers = []
        # Length of bin_peers should be multiple of 6
        while offset != len(bin_peers):
            bin_ip = struct.unpack_from('!i', bin_peers, offset)[0]
            hostname = socket.inet_ntoa(struct.pack('!i', bin_ip))
            offset += 4
            port = struct.unpack_from('!H', bin_peers, offset)[0]
            offset += 2
            peer = {'hostname': hostname, 'port': port}
            peers.append(peer)
        return peers
