#include "stream_server.h"
#include "log.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

static void read_handler_proxy(aeEventLoop* evloop, int fd, void* ud, int mask) {

}

static void write_handler_proxy(aeEventLoop* evloop, int fd, void* ud, int mask) {

}

static void accept_handler_proxy(aeEventLoop* evloop, int fd, void* ud, int mask) {

}

stream_server::stream_server(aeEventLoop* evloop)
	: evloop_(evloop), listenfd_(-1), maxconncount_(0), channels_(NULL), tcpconns_(NULL)
{

}

stream_server::~stream_server()
{
	if (listenfd_ != -1) { close(listenfd_); listenfd_ = -1; }
	if (channels_) { free(channels_); channels_ = NULL; }
	if (tcpconns_) { free(tcpconns_); tcpconns_ = NULL; }
}

bool stream_server::init(int maxccnt, int port)
{
	channels_ = (channel*)malloc(maxccnt * sizeof(channel));
	if (!channels_) {
		log_error("cannot allocate channels_");
		return false;
	}
	memset(channels_, 0, maxccnt * sizeof(channel));

	tcpconns_ = (tcpconn**)malloc(maxccnt * sizeof(tcpconn*));
	if (!tcpconns_) {
		log_error("cannot allocate tcpconns_");
		return false;
	}
    memset(tcpconns_, 0, maxccnt * sizeof(tcpconn*));

    return true;
}
