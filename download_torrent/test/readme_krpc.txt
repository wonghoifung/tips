##### bencode编码
# string:<字符串长度>:<字符串>
# integer:i<整数>e
# list:l[数据1][数据2][数据3][…]e
# dictionary:d[key1][value1][key2][value2][…]e,其中key必须是string而且按照字母顺序排序

##### dht网络的krpc协议
# t(事务id) y(消息类型q/r/e) q(请求方法名) a(请求参数) r(回复)

##### PING:
# query:
{
    't': 'aa',
    'y': 'q',  
    'q': 'ping', 
    'a': {'id': 'random_node_id'}  
}
# response:
{
    "t":"tx1", 
    "y":"r", 
    "r": {"id":"mnopqrstuvwxyz123456"}
}

##### FIND NODE:
# query:
{
    "t":"tx1", 
    "y":"q", 
    "q":"find_node", 
    "a": {"id":"abcdefghij0123456789", "target":"mnopqrstuvwxyz123456"}
}
# response:
{
    "t":"tx1", 
    "y":"r", 
    "r": {"id":"0123456789abcdefghij", "nodes": "def456..."}
}

##### GET PEERS:
# query:
{
    "t":"aa", 
    "y":"q", 
    "q":"get_peers", 
    "a": {"id":"abcdefghij0123456789", "info_hash":"mnopqrstuvwxyz123456"}
}
# response:
{
    "t":"aa", 
    "y":"r", 
    "r": {"id":"abcdefghij0123456789", "token":"aoeusnth", "values": ["axje.u", "idhtnm"]}
} # values是"CompactIP-address/portinfo"格式的peers信息
{
    "t":"aa", 
    "y":"r", 
    "r": {"id":"abcdefghij0123456789", "token":"aoeusnth", "nodes": "def456..."}
} # 没有info_hash的peers, 发送"Compactnodeinfo"格式的dht节点

##### ANNOUNCE PEER:
# query:
{
    "t":"aa", 
    "y":"q", 
    "q":"announce_peer", 
    "a": {"id":"abcdefghij0123456789", "info_hash":"mnopqrstuvwxyz123456", "port": 6881, "token": "aoeusnth"}
} # token相同才将请求节点的ip端口记录到peer的info_hash下
# response:
{
    "t":"aa", 
    "y":"r", 
    "r": {"id":"mnopqrstuvwxyz123456"}
}



