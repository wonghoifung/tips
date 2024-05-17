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
import sys

def print_torrent_file(torrent_path):
    with open(torrent_path, 'rb') as file:
        content = file.read()
    torrent_data = bencodepy.decode(content)

    # print("Torrent 文件信息:")
    # print("------------------")
    # for key, value in torrent_data.items():
    #     print(f"{key}: {value}")
    announce = torrent_data.get(b'announce', None)
    if announce:
        print("announce:{}".format(announce))

    announce_list = torrent_data.get(b'announce-list', None)
    if announce_list:
        print("announce-list:")
        for announce in announce_list:
            print("\t{}".format(announce))

    info = torrent_data.get(b'info', None)
    if info:
        print("info:")
        name = info.get(b'name', '')
        print("\tname:{}".format(name.decode('utf-8')))
        piece_length = info.get(b'piece length', '')
        print("\tpiece length:{}".format(piece_length))
        pieces = info.get(b'pieces', '')
        # print("\tpieces:{}".format(pieces))
        print("\tlen(pieces):{} len(pieces)/20:{}".format(len(pieces),len(pieces)/20)) # 20bytes per piece
        length = info.get(b'length', None)
        if length:
            print("\tlength:{}".format(length))
        else:
            files = info.get(b'files', None)
            if files:
                print("\tfiles:")
                for f in files:
                    length=f.get(b'length','')
                    path=f.get(b'path',[])
                    path_utf8=[]
                    for p in path:
                        path_utf8.append(p.decode('utf-8'))
                    print("\t\tlength:{} path:{} length/piece length:{}".format(length,path_utf8,length/piece_length))


if __name__ == "__main__":
    # torrent_path = "/home/hhf/Desktop/test/bt/downloaded.torrent"
    # print_torrent_file(torrent_path)

    if len(sys.argv) > 1:
        torrent_path = sys.argv[1]
        print("First argument: ", torrent_path)
        print_torrent_file(torrent_path)
    else:
        print("No arguments provided.")

    # torrent_path = "/home/hhf/Desktop/test/bt/downloaded.torrent"
    # print_torrent_file(torrent_path)

    # torrent_path = "/home/hhf/Desktop/test/bt/6f36c48a1b418e96b736359045a9b2cabba75281.torrent"
    # print_torrent_file(torrent_path)

    # torrent_path = "/home/hhf/Desktop/test/bt/7edaca74658f7574dc4c085ee2fc14163dbe310f.torrent"
    # print_torrent_file(torrent_path)

    # torrent_path = "/home/hhf/Desktop/test/bt/oldfox.torrent"
    # print_torrent_file(torrent_path)


# {
#     "announce": "TackerAddressString",
#     "info": {
#         "name": "NameString",
#         "piece length": 262144, 
#         "pieces": "hashString...",
#         "length": 3276800
#     }
# }

# {
#     "announce": "TackerAddressString",
#     "info": {
#         "name": "NameString",
#         "piece length": 262144, 
#         "pieces": "hashString...",
#         "files": [
#             {
#                 "length": 4096, 
#                 "path": ["Folder", "FileName1"]
#             },
#             {
#                 "length": 8192, 
#                 "path": ["Folder", "FileName2"]
#             },
#             {
#                 "length": 16384, 
#                 "path": ["Folder", "Subfolder", "FileName3"]
#             }
#         ]
#     }
# }