#include "core.h"
#include "peer.h"
#include "commands.h"
#include "stream_handler.h"

core::core(const uint32_t svrid, boost::asio::io_service& ios) : svrid_(svrid), ios_(ios)
{
  (void)svrid_;
  (void)ios_;
}

core::~core()
{

}

int core::on_connect(stream_handler* sh)
{
  return 0;
}

int core::on_message(stream_handler* sh, inmessage* inmsg)
{
  const uint16_t cmd = inmsg->command();

  // not verified messages
  switch (cmd) {
    case cmd_peer_login:
      handle_login(inmsg, sh);
  }

  peer* p = extract_peer(sh);
  if (p == NULL) {
    return -1;
  }

  // already verified messages
  switch (cmd) {
    case cmd_sock_close:
      cleanup_and_del_peer(p, inmsg);
  }

  return 0;
}

int core::on_close(stream_handler* sh, const int err)
{
  if (sh) {
    outmessage msg;
    msg.begin(cmd_sock_close);
    msg.write_int(err);
    msg.end();

    inmessage fakemsg;
    fakemsg.copy(msg.buffer(), msg.size());

    on_message(sh, &fakemsg);
  }
  return 0;
}

peer* core::add_peer(const uint32_t id, stream_handler* handler)
{
  peer* p = new peer(id, handler);
  if (p) {
    if (!p->init()) {
      delete p; p = NULL;
      return NULL;
    }
    if (!peers_.insert(std::make_pair(id, p)).second) {
      delete p; p = NULL;
      return NULL;
    }
    handler->set_ud(p);
  }
  return p;
}

peer* core::get_peer(const uint32_t id)
{
  peermap_t::iterator it = peers_.find(id);
  if (it == peers_.end()) {
    return NULL;
  }
  return it->second;
}

void core::del_peer(peer* p)
{
  peers_.erase(p->get_id());
  p->get_stream_handler()->set_ud(NULL);
  delete p;
}

int core::cleanup_and_del_peer(peer* p, inmessage* inmsg)
{
  (void)inmsg;

  // notify other modules that use this peer

  del_peer(p);
  return 0;
}

peer* core::extract_peer(stream_handler* handler)
{
  peer* p = NULL;
  if (handler && handler->get_ud()) {
    p = reinterpret_cast<peer*>(handler->get_ud());
  }
  return p;
}

peer* core::check_reconnect_peer(const uint32_t id)
{
  peer* p = get_peer(id);
  if (p) {
    outmessage outmsg;
    outmsg.begin(cmd_kick_peer);
    outmsg.write_int(id);
    outmsg.end();
    send_message(&outmsg, p->get_stream_handler());

    // notify other modules that use this peer

    p->get_stream_handler()->set_ud(NULL);
    p->set_stream_handler(NULL);
  }
  return p;
}

int core::send_message(outmessage* outmsg, stream_handler* handler)
{
  if (handler) {
    return handler->send(outmsg);
  }
  return -1;
}

int core::handle_login(inmessage* inmsg, stream_handler* handler)
{
  if (handler->get_ud()) return -1; // already login

  const uint32_t id = inmsg->read_int();
  const std::string key = inmsg->read_cstring();
  const std::string info = inmsg->read_cstring();

  // check key, process info

  peer* p = check_reconnect_peer(id);
  if (p == NULL) {
    p = add_peer(id, handler);
    if (p == NULL) return -1;
  } else {
    p->set_stream_handler(handler);
    handler->set_ud(p);
  }

  outmessage outmsg;
  outmsg.begin(cmd_peer_login);
  // some info...
  outmsg.end();
  send_message(&outmsg, handler);

  return 0;
}
