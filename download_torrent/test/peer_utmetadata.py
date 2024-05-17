import bencodepy
import socket

# 定义常量
EXT_HANDSHAKE_ID = 20
UT_METADATA_ID = 1

# magnet:?xt=urn:btih:364241889EF90FC7BD394D9B018410DBCC51E3F1
PEER_LIST=[('88.83.10.199', 6881), ('197.185.113.0', 55166), ('197.184.161.238', 23666), ('196.15.249.134', 34367), ('188.129.80.212', 63090), ('188.64.207.156', 55808), ('183.51.121.85', 80), ('181.174.102.21', 17820), ('175.107.26.133', 37663), ('155.133.17.59', 40150), ('146.70.166.206', 57210), ('129.205.195.3', 36352), ('112.211.3.104', 4658), ('103.141.102.91', 51413), ('94.140.11.238', 51413), ('65.23.228.147', 18248), ('41.4.9.246', 50582), ('31.28.213.44', 57636)]

def want_meta(peer_ip, peer_port):
    # 创建socket连接peer
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    s.connect((peer_ip, peer_port))  # 替换成实际的peer IP和端口

    # 发送扩展握手消息
    ext_handshake_msg = {
        'm': {
            'ut_metadata': 2  # 表示我们支持的ut_metadata版本
        },
        'metadata_size': 0
    }
    ext_handshake_payload = bencodepy.encode(ext_handshake_msg)
    ext_handshake_msg_len = len(ext_handshake_payload).to_bytes(4, byteorder='big')
    s.send(b"\x00\x00\x00\x00\x00\x00\x00\x00" + EXT_HANDSHAKE_ID.to_bytes(1, byteorder='big') + ext_handshake_msg_len + ext_handshake_payload)

    # 发送metadata请求
    metadata_request_msg = {
        'msg_type': UT_METADATA_ID,
        'piece': 0
    }
    metadata_request_payload = bencodepy.encode(metadata_request_msg)
    metadata_request_msg_len = len(metadata_request_payload).to_bytes(4, byteorder='big')
    s.send(b"\x00\x00\x00\x00\x00\x00\x00\x00" + UT_METADATA_ID.to_bytes(1, byteorder='big') + metadata_request_msg_len + metadata_request_payload)

    # 接收并解析metadata回复
    metadata_reply = s.recv(1024)
    metadata_reply_payload_len = int.from_bytes(metadata_reply[8:12], byteorder='big')
    metadata_reply_payload = bencodepy.decode(metadata_reply[12:12 + metadata_reply_payload_len])

    # 解析metadata
    metadata = metadata_reply_payload.get(b'metadata')
    if metadata is None:
        print("Metadata not found in the metadata reply.")
    else:
        print("Metadata", metadata)

    # 关闭socket连接
    s.close()

if __name__ == "__main__":
    for (peer_ip, peer_port) in PEER_LIST:
        try:
            if want_meta(peer_ip, peer_port):
                print('success, ip:{}, port:{}'.format(peer_ip, peer_port))
                break
        except socket.timeout:
            print('timeout, ip:{}, port:{}'.format(peer_ip, peer_port))
            continue
        except ConnectionRefusedError:
            print('refused, ip:{}, port:{}'.format(peer_ip, peer_port))
            continue
