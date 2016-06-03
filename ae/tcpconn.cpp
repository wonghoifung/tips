#include "tcpconn.h"
#include "message.h"

tcpconn::tcpconn(aeEventLoop* evloop, int fd): evloop_(evloop), fd_(fd)
{

}

tcpconn::~tcpconn()
{

}

int tcpconn::process_message(inmessage* imsg)
{

}
