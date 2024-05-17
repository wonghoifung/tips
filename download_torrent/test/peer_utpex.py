import socket
import struct
import bencodepy

# 定义常量
EXT_HANDSHAKE_ID = 20
UT_PEX_ID = 1

PEER_LIST=[('88.83.10.199', 6881), ('197.185.113.0', 55166), ('197.184.161.238', 23666), ('196.15.249.134', 34367), ('188.129.80.212', 63090), ('188.64.207.156', 55808), ('183.51.121.85', 80), ('181.174.102.21', 17820), ('175.107.26.133', 37663), ('155.133.17.59', 40150), ('146.70.166.206', 57210), ('129.205.195.3', 36352), ('112.211.3.104', 4658), ('103.141.102.91', 51413), ('94.140.11.238', 51413), ('65.23.228.147', 18248), ('41.4.9.246', 50582), ('31.28.213.44', 57636)]

def want_more_peers(peer_ip, peer_port):
    # 创建socket连接peer
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    s.connect((peer_ip, peer_port))  # 替换成实际的peer IP和端口

    # 发送扩展握手消息
    ext_handshake_msg = {
        'm': {
            'ut_pex': 1  # 表示我们支持的ut_pex版本
        }
    }
    ext_handshake_payload = bencodepy.encode(ext_handshake_msg)
    ext_handshake_msg_len = len(ext_handshake_payload).to_bytes(4, byteorder='big')
    s.send(b"\x00\x00\x00\x00\x00\x00\x00\x00" + EXT_HANDSHAKE_ID.to_bytes(1, byteorder='big') + ext_handshake_msg_len + ext_handshake_payload)

    # 接收peers信息
    peers_info = b""
    while True:
        data = s.recv(1024)
        if not data:
            break
        peers_info += data

    # 解析peers信息
    peers_payload = peers_info[20:]  # 去除握手消息头
    peers = []
    while len(peers_payload) >= 6:
        ip = socket.inet_ntoa(peers_payload[:4])  # 将4字节的IP地址转换为字符串形式
        port = struct.unpack("!H", peers_payload[4:6])[0]  # 解析端口号
        peers.append((ip, port))
        peers_payload = peers_payload[6:]

    # 打印peers列表
    print("Peers:")
    for peer in peers:
        print(peer)

    # 关闭socket连接
    s.close()
    return len(peers) > 0

if __name__ == "__main__":
    for (peer_ip, peer_port) in PEER_LIST:
        try:
            if want_more_peers(peer_ip, peer_port):
                print('success, ip:{}, port:{}'.format(peer_ip, peer_port))
                break
        except socket.timeout:
            print('timeout, ip:{}, port:{}'.format(peer_ip, peer_port))
            continue
        except ConnectionRefusedError:
            print('refused, ip:{}, port:{}'.format(peer_ip, peer_port))
            continue
