
import socket
import hashlib
import bencodepy
import threading
import time 

def calculate_info_hash_bin(torrent_path):
    with open(torrent_path, 'rb') as f:
        torrent_data = f.read()
        metadata = bencodepy.decode(torrent_data)
        info = metadata[b'info']
        info_hash = hashlib.sha1(bencodepy.encode(info)).digest()
        return info_hash

def recv_print_message(s):
    # 接收消息
    data = s.recv(1024)

    # 解释消息类型
    if len(data) < 4:
        print("Error: Received message is too short")
    else:
        # 从接收到的消息中解析消息类型
        message_length = int.from_bytes(data[:4], byteorder='big')
        if message_length == 0:
            print("Keep-alive message received")
        else:
            message_type = data[4]
            if message_type == 0:
                print("Choke message received")
            elif message_type == 1:
                print("Unchoke message received")
            elif message_type == 2:
                print("Interested message received")
            elif message_type == 3:
                print("Not interested message received")
            elif message_type == 4:
                print("Have message received")
            elif message_type == 5:
                print("Bitfield message received")
            elif message_type == 6:
                print("Request message received")
            elif message_type == 7:
                print("Piece message received")
            elif message_type == 8:
                print("Cancel message received")
            else:
                print("Unknown message type")

        # 打印整个消息
        print("Received message:", data)

def mock_handshake(s, info_hash):
    # BT handshake的格式：19字节的握手握手消息
    pstrlen = 19
    pstr = 'BitTorrent protocol'
    reserved = bytes(8)

    # return a long byte string
    handshake_message =  b''.join([
        bytes([pstrlen]),
        bytes(pstr, encoding='utf-8'),
        reserved,
        info_hash,
        b"ABCDEFGHIJKLMNOPQRST"
    ]) 

    # 发送握手消息
    s.send(handshake_message)
    
    # 接收peer的回应（通常是choke、unchoke、interested、not interested等消息）
    response = s.recv(1024)
    
    print('Received response:', response)

    # 解析返回的握手信息
    pstrlen = response[0]
    pstr = response[1:20]
    reserved = response[20:28]
    info_hash = response[28:48]
    peer_id = response[48:68]

    print("Received handshake information:")
    print("pstrlen:", pstrlen)
    print("pstr:", pstr)
    print("reserved:", reserved)
    print("info_hash:", info_hash)
    print("peer_id:", peer_id)

def mock_keepalive(s):
    # 发送Keep-Alive 消息
    keep_alive = b'\x00\x00\x00\x00'
    s.send(keep_alive)

    # 接收返回信息
    data = s.recv(4)  # Keep-Alive 消息没有数据，长度字段为4

    # 解析返回信息
    # 如果返回的数据为 b'\x00\x00\x00\x00'，表示对等体保持连接
    # 如果返回的数据不为空或者不是 b'\x00\x00\x00\x00'，表示可能有其他消息
    if data == b'\x00\x00\x00\x00':
        print("Received Keep-Alive message: Peer is still connected")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_bitfield(s):
    # 构建bitfield, 这里假设我们有10个块，可以根据实际情况更改
    bitfield_size = 10 // 8  # 10个块占用的字节数
    bitfield = b'\xFF' + b'\x00' * (bitfield_size - 1)  # 假设我们已经拥有所有的块

    # 发送Bitfield消息
    message_length = len(bitfield) + 1
    message = b'\x05' + message_length.to_bytes(4, byteorder='big') + b'\x05' + bitfield
    s.send(message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 针对Bitfield消息，返回的数据为b'\x05\x00\x00\x00'表示消息长度为5
    if data == b'\x05\x00\x00\x00':
        print("Bitfield message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_choke(s):
    # 发送choke消息
    choke_message = b'\x00\x00\x00\x01\x00'
    s.send(choke_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 对于choke消息，返回的数据为b'\x00\x00\x00\x01'表示消息长度为1
    if data == b'\x00\x00\x00\x01':
        print("Choke message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_unchoke(s):
    # 发送unchoke消息
    unchoke_message = b'\x00\x00\x00\x01\x01'
    s.send(unchoke_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 对于unchoke消息，返回的数据为b'\x00\x00\x00\x01'表示消息长度为1
    if data == b'\x00\x00\x00\x01':
        print("Unchoke message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_interested(s):
    # 发送interested消息
    interested_message = b'\x00\x00\x00\x01\x02'
    s.send(interested_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 对于interested消息，返回的数据为b'\x00\x00\x00\x01'表示消息长度为1
    if data == b'\x00\x00\x00\x01':
        print("Interested message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_not_interested(s):
    # 发送not interested消息
    not_interested_message = b'\x00\x00\x00\x01\x03'
    s.send(not_interested_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 对于not interested消息，返回的数据为b'\x00\x00\x00\x01'表示消息长度为1
    if data == b'\x00\x00\x00\x01':
        print("Not interested message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_have(s):
    # 发送have消息
    have_message = b'\x00\x00\x00\x05\x04\x00\x00\x00\x0A'  # 表示我们拥有块索引为10的块
    s.send(have_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 对于have消息，返回的数据格式和长度和输入的消息相同
    if data == b'\x00\x00\x00\x05':
        print("Have message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_request(s):
    # 构造request消息
    block_index = 10  # 请求的块索引
    block_offset = 0  # 块在文件中的偏移量
    block_length = 16384  # 块的长度，以字节为单位

    # 将索引、偏移量和长度打包成字节串
    request_message = b'\x00\x00\x00\x0D\x06\x00\x00\x00\x0A\x00\x00\x00\x00\x40\x00'  
    # 0x0D表示消息长度为13字节，0x06表示request消息，0x00\x00\x00\x0A表示块索引为10，0x00\x00\x00\x00表示偏移量，0x40\x00表示长度为16384

    # 发送request消息
    s.send(request_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 Piece 消息等

    # 解析返回信息
    # 对于request消息，返回的数据格式和长度和输入的消息相同
    if data == b'\x00\x00\x00\x0D':
        print("Request message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_piece(s):
    # 构造piece消息
    block_index = 10  # 块索引
    block_offset = 0  # 块在文件中的偏移量
    block_data = b'...'  # 块数据，这里用'...'表示实际的块数据

    # 计算消息长度
    message_length = 9 + len(block_data)

    # 将消息长度、消息类型、索引、偏移量和块数据打包成字节串
    piece_message = (
        message_length.to_bytes(4, byteorder='big') +   # 消息长度
        b'\x07' +                                       # piece消息类型为0x07
        block_index.to_bytes(4, byteorder='big') +      # 块索引
        block_offset.to_bytes(4, byteorder='big') +     # 块偏移量
        block_data                                      # 块数据
    )

    # 发送piece消息
    s.send(piece_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 interested/uninterested 消息等

    # 解析返回信息
    # 对于piece消息，返回的数据格式和长度和输入的消息相同
    if data == message_length.to_bytes(4, byteorder='big'):
        print("Piece message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def mock_cancel(s):
    # 构造cancel消息
    block_index = 10  # 取消的块索引
    block_offset = 0  # 取消的块在文件中的偏移量

    # 计算消息长度
    message_length = 13

    # 将消息长度、消息类型、索引、偏移量打包成字节串
    cancel_message = (
        message_length.to_bytes(4, byteorder='big') +   # 消息长度
        b'\x08' +                                       # cancel消息类型为0x08
        block_index.to_bytes(4, byteorder='big') +      # 取消的块索引
        block_offset.to_bytes(4, byteorder='big')       # 取消的块偏移量
    )
    # recv_print_message(s)
    # 发送cancel消息
    s.send(cancel_message)

    # 接收返回信息
    data = s.recv(4)  # 可能是一个 interested/uninterested 消息等

    # 解析返回信息
    # 对于cancel消息，返回的数据格式和长度和输入的消息相同
    if data == message_length.to_bytes(4, byteorder='big'):
        print("Cancel message sent successfully")
    else:
        print("Received message: Potentially another message from the peer. {}".format(data))
        # 进一步处理其他消息

def recv_print_thread(s):
    while True:
        try:
            recv_print_message(s)
        except socket.timeout as e:
            print('recv timeout : {}'.format(e))

if __name__ == "__main__":
    info_hash=calculate_info_hash_bin('/home/hhf/Desktop/test/bt/torrent/royal_rumble.torrent')

    # BT peer的IP地址和端口
    peer_ip = '65.23.228.147'
    peer_port = 18248

    # 创建一个socket对象
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    
    # 连接到BT peer
    s.connect((peer_ip, peer_port))

    mock_handshake(s, info_hash)
    # mock_keepalive(s)
    # mock_bitfield(s)
    # mock_choke(s)
    mock_unchoke(s)
    mock_interested(s)
    # mock_not_interested(s)
    # mock_have(s)
    # mock_request(s)
    # mock_piece(s)
    # mock_cancel(s)

    thread=threading.Thread(target=recv_print_thread, args=(s,))
    thread.start()
    thread.join()

    # 关闭socket连接
    s.close()
