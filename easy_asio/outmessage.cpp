#include "outmessage.h"

bool outmessage::write_int(int val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(int));
}

bool outmessage::write_uint(unsigned int val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(unsigned int));
}

bool outmessage::write_int64(int64_t val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(int64_t));
}

bool outmessage::write_uint64(uint64_t val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(uint64_t));
}

bool outmessage::write_long(long val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(long));
}

bool outmessage::write_ulong(unsigned long val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(unsigned long));
}

bool outmessage::write_char(char val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(char));
}

bool outmessage::write_uchar(unsigned char val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(unsigned char));
}

bool outmessage::write_short(short val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(short));
}

bool outmessage::write_ushort(unsigned short val)
{
  val = endian_op(val);
  return message_base::write_b((char*)&val, sizeof(unsigned short));
}

bool outmessage::write_front_int(int val)
{
  val = endian_op(val);
  return message_base::write_front_b((char*)&val, sizeof(int));
}

bool outmessage::write_front_uint(unsigned int val)
{
  val = endian_op(val);
  return message_base::write_front_b((char*)&val, sizeof(unsigned int));
}

bool outmessage::write_front_char(char val)
{
  val = endian_op(val);
  return message_base::write_front_b((char*)&val, sizeof(char));
}

bool outmessage::write_front_uchar(unsigned char val)
{
  val = endian_op(val);
  return message_base::write_front_b((char*)&val, sizeof(unsigned char));
}

bool outmessage::write_cstring(const char* cstr)
{
  int len = (int)strlen(cstr) ;
  write_int(len + 1) ;
  return message_base::write_b(cstr, len) && message_base::write_zero_b();
}

bool outmessage::write_string(const std::string& str)
{
  /* same way with c-string */
  return write_cstring(str.c_str());
}

bool outmessage::write_binary(const char* inbuf, int len)
{
  write_int(len) ;
  return message_base::write_b(inbuf, len) ;
}

bool outmessage::copy(const void* inbuf, int len)
{
  return message_base::copy_b(inbuf, len);
}

void outmessage::begin(short cmd, char ver, char subver)
{
  message_base::begin_b(cmd, ver, subver);
}

void outmessage::end()
{
  message_base::end_b();
}
