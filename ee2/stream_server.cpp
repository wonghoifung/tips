#include "stream_server.h"
#include "tcpconn.h"
#include "log.h"

static char errstr[1024] = { 0 };

static void accept_handler_proxy(aeEventLoop* el, int fd, void* ud, int mask)
{
	(void)el; (void)fd;
	stream_server* ss = (stream_server*)ud;
	ss->handle_accept(mask);
}

stream_server::stream_server(aeEventLoop* evloop): evloop_(evloop), listenfd_(-1), fdconns_(NULL), fdcount_(0), fdidx_(0)
{

}

stream_server::~stream_server()
{
	if (listenfd_ != -1) close(listenfd_);
	if (fdconns_) free(fdconns_);
}

void stream_server::start(int port)
{
	int listenfd_ = anetTcpServer(errstr, port, NULL, 5);
	if (ANET_ERR == listenfd_) {
		log_error("cannot init port: %d, errstr: %s", port, errstr);
		return;
	}

	if (aeCreateFileEvent(evloop_, listenfd_, AE_READABLE, accept_handler_proxy, this) == AE_ERR) {
		log_error("cannot create file event for listenfd");
		return;
	}

	fdconns_ = (tcpconn**)malloc(MAX_DESCRIPTORS * sizeof(void*));
	if (fdconns_ == NULL) {
		log_error("cannot allocate enough space for tcpconn array");
		return;
	}
    memset(fdconns_, 0, MAX_DESCRIPTORS * sizeof(void*));

	aeMain(evloop_);
}

void stream_server::handle_accept(int mask)
{
	int cfd, cport;
	char ip_addr[128] = { 0 };
	cfd = anetTcpAccept(errstr, listenfd_, ip_addr, sizeof ip_addr, &cport);
	if (ANET_ERR == cfd) {
		log_error("cannot accept new connection");
		return;
	}
	log_debug("connected from %s:%d", ip_addr, cport);

	anetSocketBuffer(cfd, 16 * 1024);

	if (anetNonBlock(errstr, cfd) == ANET_ERR) {
		log_error("cannot set new client socket:%d nonblock", cfd);
		close(cfd);
		return;
	}

	if (anetKeepAlive(errstr, cfd, 60) == ANET_ERR) {
		log_error("cannot set new client socket:%d keepalive", cfd);
		close(cfd);
		return;
	}

	tcpconn* conn = create_tcpconn(cfd);
	if (!conn) {
		log_error("cannot create tcpconn for socket:%d", cfd);
		close(cfd);
		return;
	}

	add_tcpconn(conn);

	if( aeCreateFileEvent(el, cfd, AE_READABLE, ReadFromClient, NULL) == AE_ERR )
	{
		fprintf(stderr, "client connect fail: %d\n", fd);
		close(fd);
	}
}

tcpconn* stream_server::create_tcpconn(int fd)
{

}

void stream_server::add_tcpconn(tcpconn* conn)
{
	fdcount_ += 1;
	fdidx_ += 1;
	conn->setfdidx(fdidx_);
}

void stream_server::del_tcpconn(tcpconn* conn)
{

}
