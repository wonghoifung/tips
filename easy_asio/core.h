#ifndef core_header
#define core_header

#include "message_handler.h"
#include <map>
#include <boost/noncopyable.hpp>

class peer;

class core : public message_handler, private boost::noncopyable
{
  typedef std::map<uint32_t, peer*> peermap_t;

public:
  core(const uint32_t svrid, boost::asio::io_service& ios);
  virtual ~core();

  // required by message_handler
  virtual int on_connect(stream_handler* sh);
  virtual int on_message(stream_handler* sh, inmessage* inmsg);
  virtual int on_close(stream_handler* sh, const int err);

private:
  peer* add_peer(const uint32_t id, stream_handler* handler);
  peer* get_peer(const uint32_t id);
  void del_peer(peer* p);
  int cleanup_and_del_peer(peer* p, inmessage* inmsg);
  peer* extract_peer(stream_handler* handler);
  peer* check_reconnect_peer(const uint32_t id);
  int send_message(outmessage* outmsg, stream_handler* handler);

  int handle_login(inmessage* inmsg, stream_handler* handler);
  // void handle_XXX(peer* p, inmessage* inmsg);

private:
  const uint32_t svrid_;
  boost::asio::io_service& ios_;
  peermap_t peers_;
};

#endif
