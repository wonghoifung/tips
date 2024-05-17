import os
import socket
import time
import heapq
import struct
import asyncio
import bencodepy
import ipaddress
from hashlib import sha1
import random
import json
import hashlib
import binascii

def random_node_id():
    return sha1(str(random.randint(0, 255)).encode('utf-8')).digest()

def create_get_peers_query():
    query = {
        't': b'aa', 
        'y': b'q', 
        'q': b'get_peers',  
        'a': {
            'id': b'-PY0001-123456789012',  
            # 'info_hash': b"q'\xcf\xa9cu\xa0\xae\x90\xa1\x84\x87\xfc)\x84}1\x05,>"  
            'info_hash': b'\x1e\xac\xbbi\xa3\xdfa\x9e\xf7)Y\xd5\x99&\x1d\xdbfL\xe5\xd1'
        }
    }
    return bencodepy.encode(query)

def get_peers(ip,port,recursion):
    dht_entry_ip = ip
    dht_entry_port = port

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    known_node = (dht_entry_ip, dht_entry_port)
    ping_query = create_get_peers_query()
    sock.sendto(ping_query, known_node)

    try:
        sock.settimeout(5)
        response, addr = sock.recvfrom(65536)
        print("Received response from {}:{}, addr:{}".format(dht_entry_ip, dht_entry_port, addr))
        decoded_resp = bencodepy.decode(response)
        print("Received response from {}:{}, resp:{}".format(dht_entry_ip, dht_entry_port, decoded_resp))
        t = decoded_resp.get(b't', b'').decode('utf-8')
        y = decoded_resp.get(b'y', b'').decode('utf-8')
        r_id = decoded_resp[b'r'].get(b'id', b'')
        v = decoded_resp.get(b'v')
        r_token = decoded_resp.get(b'r', {}).get(b'token', b'')
        r_values = decoded_resp.get(b'r', {}).get(b'values', None)
        if r_values:
            print("t:{} \ny:{} \nr_token:{} \nr_id:{} \nr_values:{} \nv:{}".format(t,y,r_token,r_id,r_values,v))
            for peer_info in r_values:
                if len(peer_info) == 6:
                    ip = socket.inet_ntoa(peer_info[:4])
                    port = struct.unpack("!H", peer_info[4:])[0]
                    print(f"Found peer at {ip}:{port}")
            return True
        else:
            r_nodes = decoded_resp.get(b'r', {}).get(b'nodes', None)
            print("t:{} \ny:{} \nr_token:{} \nr_id:{} \nr_nodes:{} \nv:{}".format(t,y,r_token,r_id,r_nodes,v))
            if recursion and r_nodes:
                for i in range(0, len(r_nodes), 26):
                    node_id = r_nodes[i:i+20]
                    ip = socket.inet_ntoa(r_nodes[i+20:i+24])
                    port = struct.unpack("!H", r_nodes[i+24:i+26])[0]
                    print("send get_peers to node_id:{} {}:{}".format(node_id,ip,port))
                    if get_peers(ip, port,recursion):
                        return True
                        # return False
                
    except socket.timeout:
        print("Request timed out {}:{}".format(dht_entry_ip, dht_entry_port))

    sock.close()
    return False

def calculate_sha1_hex(data):
    h = hashlib.sha1()
    h.update(data)
    return h.hexdigest()

def calculate_info_hash_hex(torrent_path):
    with open(torrent_path, 'rb') as file:
        content = file.read()
    torrent_data = bencodepy.decode(content)
    info = torrent_data.get(b'info', None)
    if info:
        return calculate_sha1_hex(bencodepy.encode(info))
    return None

def calculate_info_hash_bin(torrent_path):
    with open(torrent_path, 'rb') as f:
        torrent_data = f.read()
        metadata = bencodepy.decode(torrent_data)
        info = metadata[b'info']
        info_hash = hashlib.sha1(bencodepy.encode(info)).digest()
        return info_hash

if __name__ == "__main__":
    # bytes=b"q'\xcf\xa9cu\xa0\xae\x90\xa1\x84\x87\xfc)\x84}1\x05,>"  
    # print("info_hash len:{}".format(len(bytes)))

    # torrent_path = "/home/hhf/Desktop/test/bt/6f36c48a1b418e96b736359045a9b2cabba75281.torrent"
    # info_hash = calculate_info_hash_hex(torrent_path)
    # if info_hash:
    #     print("info_hash len:{}, info_hash:{}".format(len(info_hash), info_hash))
    # info_hash = calculate_info_hash_bin(torrent_path)
    # if info_hash:
    #     print("info_hash len:{}, info_hash:{}, hex:{}".format(len(info_hash), info_hash, info_hash.hex()))
    #     hex_info_hash = binascii.hexlify(info_hash).decode('utf-8')
    #     print("Info Hash (hex):", hex_info_hash)

    # get_peers('31.220.87.153', 10240, False)

    dht_entry_ip = "172.16.12.249"
    dht_entry_port = 6908
    get_peers(dht_entry_ip, dht_entry_port, True)



