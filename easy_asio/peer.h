#ifndef peer_header
#define peer_header

#include <boost/noncopyable.hpp>

class stream_handler;

class peer : private boost::noncopyable
{
public:
  peer(const uint32_t id, stream_handler* handler);
  ~peer();

  bool init();
  
  const uint32_t get_id() const { return peerid_; }

  void set_stream_handler(stream_handler* h) { handler_ = h; }
  stream_handler* get_stream_handler() { return handler_; }

private:
  const uint32_t peerid_;
  stream_handler* handler_;
};

#endif
