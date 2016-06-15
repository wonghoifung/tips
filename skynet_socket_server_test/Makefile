all : server client

server : socket_server.c server.c
	gcc -g -Wall -o $@ $^ -lpthread

client : socket_server.c client.c
	gcc -g -Wall -o $@ $^ -lpthread

clean:
	rm -f server client
