#ifndef TCPCONN_HEADER
#define TCPCONN_HEADER

class inmessage;

class tcpconn
{
	tcpconn(const tcpconn&);
	tcpconn& operator=(const tcpconn&);
public:
	tcpconn();
	~tcpconn();
	const uint32_t getfdidx() const { return fdidx_; }
    void setfdidx(uint32_t index) { fdidx_ = index; }
	int process_message(inmessage* imsg);
private:
	uint32_t fdidx_;
};

#endif
