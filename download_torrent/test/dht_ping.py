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

def create_ping_query():
    query = {
        't': 'aa', 
        'y': 'q', 
        'q': 'ping',
        'a': {'id': random_node_id()} 
    }
    return bencodepy.encode(query)

def ping(ip,port):
    dht_entry_ip = ip
    dht_entry_port = port

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    known_node = (dht_entry_ip, dht_entry_port)
    ping_query = create_ping_query()
    sock.sendto(ping_query, known_node)

    try:
        sock.settimeout(5)
        response, addr = sock.recvfrom(65536)
        print("Received response from {}:{}, addr:{}".format(dht_entry_ip, dht_entry_port, addr))
        decoded_resp = bencodepy.decode(response)
        print("Received response from {}:{}, resp:{}".format(dht_entry_ip, dht_entry_port, decoded_resp))
        t = decoded_resp[b't'].decode('utf-8')
        y = decoded_resp[b'y'].decode('utf-8')
        r_id = decoded_resp[b'r'][b'id']
        v = decoded_resp[b'v'].decode('utf-8')
        print("t:{} \ny:{} \nr_id:{} \nv:{}".format(t,y,r_id,v))
    except socket.timeout:
        print("Request timed out {}:{}".format(dht_entry_ip, dht_entry_port))

    sock.close()

if __name__ == "__main__":
    dht_entry_ip = "172.16.12.249"
    dht_entry_port = 6992
    ping(dht_entry_ip,dht_entry_port)

