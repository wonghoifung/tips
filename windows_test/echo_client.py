from socket import *
import sys

BUFSIZE = 1024
serverAddr = ('localhost', 8888)
clientSock = socket(AF_INET, SOCK_STREAM)

try:
    clientSock.connect(serverAddr) 
except Exception as e: 
    print 'exception catched : ' ,e
    sys.exit()

while True:
    data = raw_input('input data :')
    if not data:
        print 'client quit'
        break    
    clientSock.send(data)
    data = clientSock.recv(BUFSIZE)
    print 'recieve : ', data

clientSock.close() 
