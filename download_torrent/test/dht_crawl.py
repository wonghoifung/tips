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
# from pprint import pprint

def is_valid_ip(ip):
    try:
        # 尝试将输入作为IP地址解析
        ipaddress.ip_address(ip)
        return True
    except ValueError:
        try:
            # 尝试将输入作为域名解析
            socket.gethostbyname(ip)
            return True
        except socket.error:
            # 如果既不是有效的IP地址也不是有效的域名
            print(f'Invalid IP or domain name: {ip}')
            return False

# def upnp_port_mapping(port, duration=3600):
#     upnp = miniupnpc.UPnP()
#     upnp.discoverdelay = 10
#     upnp.discover()
#     upnp.selectigd()
#     upnp.addportmapping(port, 'UDP', upnp.lanaddr, port, 'DHT Node', '')
#     print(f"UPnP port mapping: External {upnp.externalipaddress()}:{port} to internal {upnp.lanaddr}:{port}")
#     return upnp


class Node:
    def __init__(self, node_id, ip, port):
        self.node_id = node_id
        self.ip = ip
        self.port = port
        self.last_active = time.time()  # 记录最后活跃时间

    def __eq__(self, other):
        if not isinstance(other, Node):
            return NotImplemented
        return self.node_id == other.node_id and self.ip == other.ip and self.port == other.port

    def __hash__(self):
        return hash((self.node_id, self.ip, self.port))

    def update_last_active(self):
        self.last_active = time.time()  # 更新活跃时间

    def display_info(self):
        return f"Node ID: {self.node_id.hex()}, IP: {self.ip}, Port: {self.port}"

    def display(self):
        print(
            f"Node ID: {self.node_id.hex()}, IP: {self.ip}, Port: {self.port}")



class RoutingTable:
    MAX_NODES = 100000
    def __init__(self, node_id):
        self.nodes = []
        self.closest_nodes_heap = []
        self.node_id = node_id

    def add_node(self, node):
        if node not in self.nodes and self.is_node_valid(node):
            if len(self.nodes) < self.MAX_NODES:
                self.nodes.append(node)
                self.update_closest_nodes_heap(node)
            else:
                self.replace_oldest_node(node)

    def is_node_valid(self, node):
        return is_valid_ip(node.ip) and (time.time() - node.last_active < 1800)

    def replace_oldest_node(self, new_node):
        oldest_node = min(self.nodes, key=lambda node: node.last_active)
        if oldest_node.last_active < new_node.last_active:
            # 移除最老的节点并添加新节点
            self.nodes.remove(oldest_node)
            self.nodes.append(new_node)
            # 更新最近节点的小根堆
            self.update_closest_nodes_heap(new_node)

    def find_node_by_id(self, node_id):
        for node in self.nodes:
            if node.node_id == node_id:
                return node
        return None

    def get_nodes(self):
        return self.nodes

    def display_all_nodes(self):
        for node in self.nodes:
            print(node.display_info())

    def update_closest_nodes_heap(self, new_node):
        # 假设我们想维护一个包含最近8个节点的小根堆
        k = 8
        # 计算新节点与自身节点ID的距离
        distance = self.xor_distance(self.node_id, new_node.node_id)
        # 如果堆未满，直接添加新节点
        if len(self.closest_nodes_heap) < k:
            heapq.heappush(self.closest_nodes_heap, (distance, new_node))
        else:
            # 如果新节点比堆顶的节点更近，替换并调整堆
            heapq.heappushpop(self.closest_nodes_heap, (distance, new_node))

    def find_closest_nodes(self, target_node_id, k=8):
        # 直接从小根堆中获取最近的k个节点
        return [node for _, node in heapq.nsmallest(k, self.closest_nodes_heap)]

    @staticmethod
    def xor_distance(node_id1, node_id2):
        return int.from_bytes(node_id1, byteorder='big') ^ int.from_bytes(node_id2, byteorder='big')

    def remove_expired_nodes(self, timeout=1800):  # 例如30分钟为过期
        current_time = time.time()
        self.nodes = [node for node in self.nodes if current_time - node.last_active < timeout]


def create_find_node_query(target_node_id):
    """创建find_node查询的bencode编码消息"""
    query = {
        't': b'aa',  # 事务ID
        'y': b'q',   # 消息类型：q = 查询
        'q': b'find_node',  # 查询类型
        'a': {
            'id': random_node_id(),  # 发起查询的节点ID
            'target': target_node_id  # 目标节点ID
        }
    }
    return bencodepy.encode(query)


def encode_nodes(nodes):
    """ 将节点列表编码为一个紧凑的字节串 """
    encoded = b''
    for node in nodes:
        # 确保节点 ID 是 20 字节
        if len(node.node_id) != 20 or not is_valid_ip(node.ip):
            continue

        # IP 地址转换为字节
        ip_encoded = socket.inet_aton(node.ip)

        # 端口转换为字节
        port_encoded = struct.pack("!H", node.port)

        # 连接节点 ID, IP 和端口
        encoded += node.node_id + ip_encoded + port_encoded
    return encoded

def print_query(query):
    queryjson={}
    arguments={}
    # transaction-id
    t = query.get(b't', b'')
    queryjson['t'] = t.decode('utf-8')
    # message-type q/r/e
    y = query.get(b'y', b'')
    queryjson['y'] = y.decode('utf-8')
    # method-name
    q = query.get(b'q', b'')
    queryjson['q'] = q.decode('utf-8')
    # method-arguments
    a = query.get(b'a', {})
    for key,value in a.items():
        arguments[key.decode('utf-8')]=value.decode('utf-8')
    queryjson['a'] = arguments

class DHTProtocol:
    def __init__(self, node_id, routing_table):
        self.node_id = node_id
        self.routing_table = routing_table
        self.transport = None
        self.info_hash_list = set()

    def error_received(self, exc):
        print(f"Error received: {exc}")

    def connection_made(self, transport):
        self.transport = transport

    def datagram_received(self, data, addr):
        # 异步处理接收到的数据
        asyncio.create_task(self.handle_received_data(data, addr))

    def pause_writing(self):
        print("[pause_writing]")

    def resume_writing(self):
        print("[resume_writing]")

    async def send_message(self, message, address):
        self.transport.sendto(message, address)

    async def send_find_node(self, target_node_id):
        #self.routing_table.remove_expired_nodes()  # 清理过期节点
        query = create_find_node_query(target_node_id)
        # 发送给路由表中的每个节点
        for node in self.routing_table.get_nodes():
            # print("send find_node to {}:{}".format(node.ip,node.port))
            await self.send_message(query, (node.ip, node.port))
	
    def update_node_activity(self, node_id):
    	node = self.routing_table.find_node_by_id(node_id)
    	if node:
        	node.update_last_active()  # 更新节点的最后活跃时间
    	# else:
        # 	print("Node with ID {} not found in routing table.".format(node_id.hex()))

    def process_node_id(self, decoded_data):
		# 提取消息类型
    	message_type = decoded_data.get(b'y', b'')
    	# 响应类型消息
    	if message_type == b'r':
        	node_id = decoded_data.get(b'r', {}).get(b'id', None)
    	# 查询类型消息
    	elif message_type == b'q':
        	node_id = decoded_data.get(b'a', {}).get(b'id', None)
    	else:
        	return  # 如果消息类型不是 'q' 或 'r'，则直接返回

    	# 更新节点活跃状态
    	if node_id:
        	self.update_node_activity(node_id)

    async def handle_received_data(self, data, addr):
        # 解码接收到的数据
        try:
            decoded_data = bencodepy.decode(data)
            if not isinstance(decoded_data, dict):
                print("Received data is not a dictionary")
                return
        except (bencodepy.DecodingError, ValueError):
            print("Received invalid data")
            return

		# 处理并更新节点活跃状态
        self.process_node_id(decoded_data)

        # 检查消息类型
        message_type = decoded_data.get(b'y', b'')

        # 处理查询请求
        if message_type == b'q':
            await self.handle_query(decoded_data, addr)

        # 处理响应消息
        elif message_type == b'r':
            self.handle_response(decoded_data, addr)

    async def handle_query(self, query, addr):
        query_type = query.get(b'q', b'')
        # print("--------------------{")
        # print("query: {}".format(query))
        # print("====================")
        # print_query(query)
        # print("--------------------}")
        if query_type == b'ping':
            await self.handle_ping(query, addr)
        elif query_type == b'find_node':
            await self.handle_find_node(query, addr)
        elif query_type == b'get_peers':
            await self.handle_get_peers(query, addr)
        elif query_type == b'announce_peer':
            await self.handle_announce_peer(query, addr)

    async def handle_ping(self, query, addr):
        print(f"received ping from {addr[0]}:{addr[1]}")
        response = {
            't': query[b't'],  # 复用请求的事务ID
            'y': b'r',
            'r': {'id': self.node_id}
        }
        self.transport.sendto(bencodepy.encode(response), addr)

    async def handle_find_node(self, query, addr):
        # 提取目标节点ID
        target_node_id = query[b'a'][b'target']
        print("receive find_node {} from {}:{}".format(target_node_id.hex(),addr[0],addr[1]))
        # 找到距离目标节点ID最近的节点
        closest_nodes = self.routing_table.find_closest_nodes(target_node_id)
        response = {
            't': query[b't'],
            'y': b'r',
            'r': {
                'id': self.node_id,
                'nodes': encode_nodes(closest_nodes)  # 编码节点信息
            }
        }
        self.transport.sendto(bencodepy.encode(response), addr)

    async def handle_get_peers(self, query, addr):
        infohash = query[b'a'][b'info_hash']
        self.info_hash_list.add(infohash.hex())
        print("receive get_peers {} info_hash_list size {} from {}:{}".format(infohash.hex(), 
            len(self.info_hash_list),addr[0],addr[1]))
        # 此处实现查找infohash的逻辑
        # 如果找到peers，则返回它们；如果没有，则返回最近的节点
        token = os.urandom(8)
        closest_nodes = self.routing_table.find_closest_nodes(infohash)
        response = {
            't': query[b't'],
            'y': b'r',
            'r': {
                'id': self.node_id,
                'token': token,  # 返回生成的token
                # 'values': [peers],  # 如果有peers信息
                'nodes': encode_nodes(closest_nodes)  # 如果没有peers信息
            }
        }
        self.transport.sendto(bencodepy.encode(response), addr)

    async def handle_announce_peer(self, query, addr):
        infohash = query[b'a'][b'info_hash']
        self.info_hash_list.add(infohash.hex())
        print('receive annouce_peer: {}, info_hash_list size: {} from {}:{}'.format(infohash.hex(), 
                                        len(self.info_hash_list),addr[0],addr[1]))

        # 此处实现处理announce_peer的逻辑
        # 通常，这包括更新您的数据库，以反映节点宣布拥有特定资源
        response = {
            't': query[b't'],
            'y': b'r',
            'r': {'id': self.node_id}
        }
        self.transport.sendto(bencodepy.encode(response), addr)

    def handle_response(self, response, addr):
        # print("response: {}".format(response))
        # print("response from {}:{}".format(addr[0],addr[1]))
        # 解析响应中的节点信息
        nodes_info = response.get(b'r', {}).get(b'nodes', None)
        if nodes_info:
            self.process_nodes_info(nodes_info)

            # 解析响应中的peers信息
        peers_info = response.get(b'r', {}).get(b'values', None)
        if peers_info:
            self.process_peers_info(peers_info)

    def process_nodes_info(self, nodes_info):
        # 解码节点信息并更新路由表
        for i in range(0, len(nodes_info), 26):
            node_id = nodes_info[i:i+20]
            ip = socket.inet_ntoa(nodes_info[i+20:i+24])
            port = struct.unpack("!H", nodes_info[i+24:i+26])[0]
            node = Node(node_id, ip, port)
            self.routing_table.add_node(node)

    def process_peers_info(self, peers_info):
        # 解析peers信息
        for peer_info in peers_info:
            # 每个peer信息通常是一个包含IP和端口的字节串
            if len(peer_info) == 6:
                ip = socket.inet_ntoa(peer_info[:4])
                port = struct.unpack("!H", peer_info[4:])[0]
                # 这里可以进一步处理peers信息，例如添加到一个列表或数据库中
                print(f"Found peer at {ip}:{port}")


async def create_udp_server(node_id, routing_table):
    loop = asyncio.get_running_loop()
    transport, protocol = await loop.create_datagram_endpoint(
        lambda: DHTProtocol(node_id, routing_table),
        local_addr=('0.0.0.0', 6881))
    return transport, protocol


def create_ping_query():
    """创建ping查询的bencode编码消息"""
    query = {
        't': 'aa',  # 事务ID
        'y': 'q',   # 消息类型：q = 查询
        'q': 'ping',  # 查询类型
        'a': {'id': random_node_id()}  # 查询参数：随机节点ID
    }
    return bencodepy.encode(query)


def JoinDHT(routing_table):
    dht_entry_ip = "dht.transmissionbt.com"
    # dht_entry_ip = "172.16.12.249"
    dht_entry_port = 6881

    # 加入DHT网络：向一个已知的DHT节点发送ping请求
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    known_node = (dht_entry_ip, dht_entry_port)
    ping_query = create_ping_query()
    sock.sendto(ping_query, known_node)

    # 等待响应
    target_node_id = ''
    try:
        response, addr = sock.recvfrom(65536)
        print("Received response from {}:{}, addr:{}".format(dht_entry_ip, dht_entry_port, addr))
        decoded_resp = bencodepy.decode(response)
        print("Received response from {}:{}, resp:{}".format(dht_entry_ip, dht_entry_port, decoded_resp))
        # print(json.dumps(decoded_resp.decode('utf-8')))
        print_query(decoded_resp)
        target_node_id = decoded_resp[b'r'][b'id']

    except socket.timeout:
        print("Request timed out {}:{}".format(dht_entry_ip, dht_entry_port))

    bootstrap_node = Node(target_node_id, dht_entry_ip, dht_entry_port)
    routing_table.add_node(bootstrap_node)
    sock.close()


def random_node_id():
    """生成随机节点ID"""
    return sha1(str(random.randint(0, 255)).encode('utf-8')).digest()


async def main():
    node_id = random_node_id()
    routing_table = RoutingTable(node_id)
    _, protocol = await create_udp_server(node_id, routing_table)

    # 加入DHT网络
    JoinDHT(routing_table)

    # 无限循环以定期发送 find_node 查询
    while True:
		# 获取当前路由表的节点数量
        num_nodes = len(routing_table.get_nodes())
        print("nodes_size {}".format(num_nodes))
        # routing_table.display_all_nodes()

        # 根据节点数量确定发送查询的间隔
        if num_nodes < 10000:       # 节点较少时
            interval = 1          # 更频繁地发送查询
        elif num_nodes < 50000:     # 节点数量中等
            interval = 3         # 适中的间隔
        else:                     # 节点数量较多
            interval = 10         # 较少的频率

        target_node_id = random_node_id()  # 为每次查询生成新的随机目标节点ID
        await protocol.send_find_node(target_node_id)

        # 等待一段时间再发送下一个查询
        await asyncio.sleep(interval)

if __name__ == "__main__":
    asyncio.run(main())