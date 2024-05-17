
handshake 0x13 BitTorrent protocol \0\0\0\0\0\0\0\0 info_hash peerid

keepalive 长度0

message: len typeid payload 

0 choke
1 unchoke
2 interested
3 not interested
4 have
5 bitfield
6 request
7 piece
8 cancel

当连接建立后，默认状态为阻塞和不感兴趣。choke & not interested

------------------

0x14 extended