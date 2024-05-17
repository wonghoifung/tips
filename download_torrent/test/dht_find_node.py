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

def create_find_node_query():
    query = {
        't': b'aa', 
        'y': b'q', 
        'q': b'find_node', 
        'a': {
            'id': random_node_id(), 
            'target': random_node_id()
        }
    }
    return bencodepy.encode(query)

def find_node(ip,port):
    dht_entry_ip = ip
    dht_entry_port = port

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    known_node = (dht_entry_ip, dht_entry_port)
    ping_query = create_find_node_query()
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
        r_nodes = decoded_resp[b'r'].get(b'nodes', b'')
        v = decoded_resp.get(b'v').decode('utf-8')
        print("t:{} \ny:{} \nr_id:{} \nr_nodes:{} \nv:{}".format(t,y,r_id,r_nodes,v))
    except socket.timeout:
        print("Request timed out {}:{}".format(dht_entry_ip, dht_entry_port))

    sock.close()

if __name__ == "__main__":
    dht_entry_ip = "172.16.12.249"
    dht_entry_port = 6992
    find_node(dht_entry_ip,dht_entry_port)

