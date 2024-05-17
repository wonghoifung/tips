import socket
import struct
import random
import hashlib
import bencodepy

tracker_list=[]

def calculate_info_hash_bin(torrent_path):
    with open(torrent_path, 'rb') as f:
        torrent_data = f.read()
        metadata = bencodepy.decode(torrent_data)
        announce_list = metadata.get(b'announce-list', None)
        if announce_list:
            for announce in announce_list:
                tracker_list.append(announce[0].decode('utf-8'))
        info = metadata[b'info']
        info_hash = hashlib.sha1(bencodepy.encode(info)).digest()
        return info_hash
    
def udp_announce(tracker_url, info_hash, peer_id, port):
    try:
        if tracker_url == None:
            return []
        # 解析 Tracker URL
        url_parts = tracker_url.split(':')
        host = url_parts[0]
        port = int(url_parts[1])

        # 创建 UDP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.settimeout(10)

        # 发送请求
        connection_id = 0x41727101980  # 固定的初始连接 ID
        transaction_id = random.randint(0, 65535)  # 生成随机的事务 ID

        # 发送连接请求
        connect_msg = struct.pack("!QII", connection_id, 0, transaction_id)
        sock.sendto(connect_msg, (host, port))

        # 接收响应
        response, _ = sock.recvfrom(2048)
        print('0 response:{}'.format(response))
        # connection_id_resp = struct.unpack("!IIQ", response[:12])[2]
        action, transition_id, connection_id_resp = struct.unpack('!LLQ', response)

        # 构造并发送 announce 请求
        action = 1  # 1 表示 announce
        # info_hash = bytes.fromhex(info_hash)  # 转换 info_hash 为字节串
        # peer_id = bytes.fromhex(peer_id)  # 转换 peer_id 为字节串
        downloaded = 0
        left = 0
        uploaded = 0
        event = 0  # 0 表示 none
        ip = 0  # 0 表示默认
        key = random.randint(0, 65535)
        num_want = 200  # 请求的 peer 数量
        port = port

        announce_msg = struct.pack("!QII20s20sQQQIIIIH", connection_id_resp, action, transaction_id,
                                   info_hash, peer_id, downloaded, left, uploaded, event, ip, key, num_want, port)
        sock.sendto(announce_msg, (host, port))

        # 接收并解析 announce 响应
        response, _ = sock.recvfrom(2048)
        print('1 response:{}'.format(response))
        peers = []
        if len(response) > 20:  # 如果响应包含了 peers 信息
            for i in range(20, len(response), 6):
                ip = socket.inet_ntoa(response[i:i+4])  # 解析 IP 地址
                port = struct.unpack("!H", response[i+4:i+6])[0]  # 解析端口号
                peers.append((ip, port))

        return peers
    except Exception as e:
        print("Error:", e)
        return []

if __name__ == '__main__':
    # torrent_path = "/home/hhf/Desktop/test/bt/6f36c48a1b418e96b736359045a9b2cabba75281.torrent"
    torrent_path = "/home/hhf/Desktop/test/bt/torrent/royal_rumble.torrent"
    info_hash = calculate_info_hash_bin(torrent_path)
    # print (tracker_list)
    while True:
        if len(tracker_list) == 0:
            print('no tracker left')
            break
        tracker_url=tracker_list.pop()
        if not tracker_url.startswith('udp'):
            continue

        tracker_host_port = tracker_url[len('udp://'):][0:-9]
        print('try tracker:{},{}'.format(tracker_host_port, tracker_url))

        peer_id = b"ABCDEFGHIJKLMNOPQRST"  # 随机生成的 peer ID
        port = 6881  # 本地监听端口

        peers = udp_announce(tracker_host_port, info_hash, peer_id, port)
        print("Peers:", peers)
        if len(peers) > 0:
            break
