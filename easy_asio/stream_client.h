#ifndef stream_client_header
#define stream_client_header

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include "stream_base.h"
#include "message_handler.h"

class stream_handler;
class inmessage;
class outmessage;

typedef boost::function<int (stream_handler*, inmessage*)> message_callback;
typedef boost::function<int (stream_handler*)> close_callback;
typedef boost::function<int (stream_handler*)> connect_callback;

class stream_client : public message_handler, private boost::noncopyable
{
public:
  stream_client();
  virtual ~stream_client();

  // required by message_handler
  virtual int on_connect(stream_handler*);
  virtual int on_message(stream_handler*, inmessage*);
  virtual int on_close(stream_handler*, const int);

  void connect(boost::asio::io_service& io_service, const std::string& host, const std::string& port);
  void send(outmessage* outmsg);

  void set_message_callback(message_callback cb) { message_cb_ = cb; }
  void set_close_callback(close_callback cb) { close_cb_ = cb; }
  void set_connect_callback(connect_callback cb) { connect_cb_ = cb; }

private:
  void try_connect(const std::string& host, const std::string& port);
  void on_alarm(const boost::system::error_code& error);

private:
  message_callback message_cb_;
  close_callback close_cb_;
  connect_callback connect_cb_;

  boost::asio::deadline_timer* timer_;
  boost::asio::io_service* io_service_;
  stream_handler* handler_;

  std::string host_;
  std::string port_;
};

#endif
