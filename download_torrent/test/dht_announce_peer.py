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

def random_node_id():
    return sha1(str(random.randint(0, 255)).encode('utf-8')).digest()

def create_announce_peer_query(id,token):
    query = {
        't': b'bb', 
        'y': b'q', 
        'q': b'announce_peer', 
        'a': {
            'id': id, 
            'info_hash': b"q'\xcf\xa9cu\xa0\xae\x90\xa1\x84\x87\xfc)\x84}1\x05,>",
            'port': 6881,
            'token': token
        }
    }
    return bencodepy.encode(query)

def create_get_peers_query(id):
    query = {
        't': b'aa', 
        'y': b'q', 
        'q': b'get_peers',  
        'a': {
            'id': id,  
            'info_hash': b"q'\xcf\xa9cu\xa0\xae\x90\xa1\x84\x87\xfc)\x84}1\x05,>"  
        }
    }
    return bencodepy.encode(query)

def get_peers_token(ip,port,id):
    dht_entry_ip = ip
    dht_entry_port = port

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    known_node = (dht_entry_ip, dht_entry_port)
    ping_query = create_get_peers_query(id)
    sock.bind(('0.0.0.0',6881))
    sock.sendto(ping_query, known_node)

    try:
        sock.settimeout(5)
        response, addr = sock.recvfrom(65536)
        # print("Received get_peers response from {}:{}, addr:{}".format(dht_entry_ip, dht_entry_port, addr))
        decoded_resp = bencodepy.decode(response)
        # print("Received get_peers response from {}:{}, resp:{}".format(dht_entry_ip, dht_entry_port, decoded_resp))
        t = decoded_resp.get(b't', b'').decode('utf-8')
        y = decoded_resp.get(b'y', b'').decode('utf-8')
        r_id = decoded_resp[b'r'].get(b'id', b'')
        v = decoded_resp.get(b'v')
        r_token = decoded_resp.get(b'r', {}).get(b'token', b'')
        return r_token
    except socket.timeout:
        print("Request get_peers timed out {}:{}".format(dht_entry_ip, dht_entry_port))
    sock.close()
    return None

def announce_peer(ip,port,id,token):
    dht_entry_ip = ip
    dht_entry_port = port

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    known_node = (dht_entry_ip, dht_entry_port)
    ping_query = create_announce_peer_query(id,token)
    sock.bind(('0.0.0.0',6881))
    sock.sendto(ping_query, known_node)

    try:
        sock.settimeout(5)
        response, addr = sock.recvfrom(65536)
        print("Received announce_peer response from {}:{}, addr:{}".format(dht_entry_ip, dht_entry_port, addr))
        decoded_resp = bencodepy.decode(response)
        print("Received announce_peer response from {}:{}, resp:{}".format(dht_entry_ip, dht_entry_port, decoded_resp))
        t = decoded_resp.get(b't', b'').decode('utf-8')
        y = decoded_resp.get(b'y', b'').decode('utf-8')
        r_id = decoded_resp[b'r'].get(b'id', b'')
        v = decoded_resp.get(b'v').decode('utf-8')
        print("t:{} \ny:{} \nr_id:{} \nv:{}".format(t,y,r_id,v))
    except socket.timeout:
        print("Request announce_peer timed out {}:{}".format(dht_entry_ip, dht_entry_port))

    sock.close()

if __name__ == "__main__":
    id = random_node_id()
    dht_entry_ip = "172.16.12.249"
    dht_entry_port = 6985
    token = get_peers_token(dht_entry_ip,dht_entry_port,id)
    announce_peer(dht_entry_ip,dht_entry_port,id, token)

