#ifndef MESSAGE_HEADER
#define MESSAGE_HEADER

#include <memory>
#include <string>
#include <cstdarg>
#include <string.h>
#include "EndianOp.h"

enum { c_default_version = 1, c_default_subversion = 1 };
enum { c_header_size = 9, c_buffer_size = 1024 * 12 };

/* header: | flag_2 | cmd_2 | version_1 | subversion_1 | bodylen_2 | checkcode_1 | */
class Message
{
public:
    Message() {

    }

    virtual ~Message() {

    }

    char* buffer() { 
    	return buffer_; 
    }

    const char* cbuffer() const	{ 
    	return buffer_; 
    }

    int size() const { 
    	return size_; 
    }

    short command() {
	    short cmd;
	    read_header_b((char*)&cmd, sizeof(short), 2);
	    return EndianOp(cmd);
	}

    char version() {
	    char v;
	    read_header_b(&v, sizeof(char), 4);
	    return EndianOp(v);
	}

    char subversion() {
	    char sv;
	    read_header_b(&sv, sizeof(char), 5);
	    return EndianOp(sv);
	}

    short body_length() {
	    short bodylen;
	    read_header_b((char*)&bodylen, sizeof(short), 6);
	    return EndianOp(bodylen);
	}

    unsigned char check_code() {
	    unsigned char code;
	    read_header_b((char*)&code, sizeof(unsigned char), 8);
	    return EndianOp(code);
	}

    void reset() {
	    readptr_ = c_header_size;
	    size_ = c_header_size;
	}

protected:
    bool copy_b(const void* inbuf, int len) {
	    if(len > c_buffer_size)
	    { return false; }
	    reset();
	    memcpy(buffer_, inbuf, len);
	    size_ = len;
	    return true;
	}

    void begin_b(short cmd, char ver, char subver) {
	    reset();
	    const char flag[] = "GP";
		cmd = EndianOp(cmd);
		ver = EndianOp(ver);
		subver = EndianOp(subver);
	    write_header_b(flag, sizeof(char)*2, 0);
	    write_header_b((char*)&cmd, sizeof(short), 2);
	    write_header_b(&ver, sizeof(char), 4);
	    write_header_b(&subver, sizeof(char), 5);
	}

    void end_b() {
	    short bodylen = static_cast<short>(size_ - c_header_size);
			bodylen = EndianOp(bodylen);
	    write_header_b((char*)&bodylen, sizeof(short), 6);
	    unsigned char code = 0; // TODO do verification
			code = EndianOp(code);
	    write_header_b((char*)&code, sizeof(unsigned char), 8);
	}

    bool read_b(char* outbuf, int len) {
	    if((len + readptr_) > size_ || (len + readptr_) > c_buffer_size)
	    { return false; }
	    memcpy(outbuf, buffer_ + readptr_, len);
	    readptr_ += len;
	    return true;
	}

    bool read_del_b(char* outbuf, int len) {
	    if(!read_b(outbuf, len))
	    { return false; }
	    memmove(buffer_ + readptr_ - len, buffer_ + readptr_, size_ - readptr_);
	    readptr_ -= len;
	    size_ -= len;
	    end_b();
	    return true;
	}

    void read_undo_b(int len) {
	    readptr_ -= len;
	}

    char* read_bytes_b(int len) {
	    if((len + readptr_) > size_)
	    { return NULL; }
	    char* p = &buffer_[readptr_];
	    readptr_ += len;
	    return p;
	}

    bool write_b(const char* inbuf, int len) {
	    if((size_ < 0) || ((len + size_) > c_buffer_size))
	    { return false; }
	    memcpy(buffer_+size_, inbuf, len);
	    size_ += len;
	    return true;
	}

    bool write_front_b(const char* inbuf, int len) {
	    if((len + size_) > c_buffer_size)
	    { return false; }
	    memmove(buffer_ + c_header_size + len, buffer_ + c_header_size, size_ - c_header_size);
	    memcpy(buffer_ + c_header_size, inbuf, len);
	    size_ += len;
	    end_b();
	    return true;
	}

    bool write_zero_b() {
	    if((size_ + 1) > c_buffer_size)
	    { return false; }
	    memset(buffer_+size_, '\0', sizeof(char));
	    ++size_;
	    return true;
	}

    void read_header_b(char* outbuf, int len, int pos) {
	    if(pos > 0 || pos+len < c_header_size)
	    { memcpy(outbuf, buffer_+pos, len); }
	}

    void write_header_b(const char* inbuf, int len, int pos) {
	    if(pos > 0 || pos+len < c_header_size)
	    { memcpy(buffer_+pos, inbuf, len); }
	}

private:
    char buffer_[c_buffer_size];
    int size_ ;
    int readptr_;
};

class InMessage : public Message
{
public:
    int read_int() {
	    int val(-1);
	    Message::read_b((char*)&val, sizeof(int));
	    return EndianOp(val);
	}

    int read_int_and_repack() {
	    int val(-1);
	    Message::read_del_b((char*)&val, sizeof(int));
	    return EndianOp(val);
	}

    unsigned int read_uint() {
	    unsigned int val(0);
	    Message::read_b((char*)&val, sizeof(unsigned int));
	    return EndianOp(val);
	}

    int64_t read_int64() {
	    int64_t val(0);
	    Message::read_b((char*)&val, sizeof(int64_t));
	    return EndianOp(val);
	}

    uint64_t read_uint64() {
	    uint64_t val(0);
	    Message::read_b((char*)&val, sizeof(uint64_t));
	    return EndianOp(val);
	}

    long read_long() {
	    long val(0);
	    Message::read_b((char*)&val, sizeof(long));
	    return EndianOp(val);
	}

    unsigned long read_ulong() {
	    unsigned long val(0);
	    Message::read_b((char*)&val, sizeof(unsigned long));
	    return EndianOp(val);
	}

    short read_short() {
	    short val(-1);
	    Message::read_b((char*)&val, sizeof(short));
	    return EndianOp(val);
	}

    unsigned short read_ushort() {
	    unsigned short val(0);
	    Message::read_b((char*)&val, sizeof(unsigned short));
	    return EndianOp(val);
	}

    char read_char() {
	    char val(0);
	    Message::read_b((char*)&val, sizeof(char));
	    return EndianOp(val);
	}

    unsigned char  read_uchar() {
	    unsigned char val(0);
	    Message::read_b((char*)&val, sizeof(unsigned char));
	    return EndianOp(val);
	}

    char* read_cstring() {
	    int len = read_int();
	    if(len <= 0 || len >= c_buffer_size)
	        return NULL;
	    return Message::read_bytes_b(len);
	}

    int read_binary(char* outbuf, int maxlen) {
	    int len = read_int();
	    if(len <= 0)
	        return -1;
	    if(len > maxlen)
	    {
	        Message::read_undo_b(sizeof(int));
	        return -1;
	    }
	    if(Message::read_b(outbuf, len))
	        return len ;
	    return 0;
	}

    bool copy(const void* inbuf, int len) {
	    return Message::copy_b(inbuf, len);
	}

    void begin(short cmd, char ver = c_default_version, char subver = c_default_subversion) {
	    Message::begin_b(cmd, ver, subver);
	}

    void end() {
	    Message::end_b();
	}

    /* for parser to build packet */
    bool append(const char* inbuf, int len) { 
    	return Message::write_b(inbuf, len); 
    }
};

class OutMessage : public Message
{
public:
    bool write_int(int val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(int));
	}

    bool write_uint(unsigned int val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(unsigned int));
	}

    bool write_int64(int64_t val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(int64_t));
	}

    bool write_uint64(uint64_t val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(uint64_t));
	}

    bool write_long(long val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(long));
	}

    bool write_ulong(unsigned long val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(unsigned long));
	}

    bool write_char(char val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(char));
	}

    bool write_uchar(unsigned char val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(unsigned char));
	}

    bool write_short(short val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(short));
	}

    bool write_ushort(unsigned short val) {
	  val = EndianOp(val);
	  return Message::write_b((char*)&val, sizeof(unsigned short));
	}

    bool write_front_int(int val) {
	  val = EndianOp(val);
	  return Message::write_front_b((char*)&val, sizeof(int));
	}

    bool write_front_uint(unsigned int val) {
	  val = EndianOp(val);
	  return Message::write_front_b((char*)&val, sizeof(unsigned int));
	}

    bool write_front_char(char val) {
	  val = EndianOp(val);
	  return Message::write_front_b((char*)&val, sizeof(char));
	}

    bool write_front_uchar(unsigned char val) {
	  val = EndianOp(val);
	  return Message::write_front_b((char*)&val, sizeof(unsigned char));
	}

    bool write_cstring(const char* cstr) {
	  int len = (int)strlen(cstr) ;
	  write_int(len + 1) ;
	  return Message::write_b(cstr, len) && Message::write_zero_b();
	}

    bool write_string(const std::string& str) {
	  /* same way with c-string */
	  return write_cstring(str.c_str());
	}

    bool write_binary(const char* inbuf, int len) {
	  write_int(len) ;
	  return Message::write_b(inbuf, len) ;
	}

    bool copy(const void* inbuf, int len) {
	  return Message::copy_b(inbuf, len);
	}

    void begin(short cmd, char ver = c_default_version, char subver = c_default_subversion) {
	  Message::begin_b(cmd, ver, subver);
	}

    void end() {
	  Message::end_b();
	}
};

#endif
