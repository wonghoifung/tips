#include "peer.h"

peer::peer(const uint32_t id, stream_handler* handler) : peerid_(id), handler_(handler)
{

}

peer::~peer()
{

}

bool peer::init()
{
  // init
  return true;
}
