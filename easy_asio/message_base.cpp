#include "message_base.h"

/* header: | flag_2 | cmd_2 | version_1 | subversion_1 | bodylen_2 | checkcode_1 | */

short message_base::command()
{
    short cmd;
    read_header_b((char*)&cmd, sizeof(short), 2);
    return endian_op(cmd);
}

char message_base::version()
{
    char v;
    read_header_b(&v, sizeof(char), 4);
    return endian_op(v);
}

char message_base::subversion()
{
    char sv;
    read_header_b(&sv, sizeof(char), 5);
    return endian_op(sv);
}

short message_base::body_length()
{
    short bodylen;
    read_header_b((char*)&bodylen, sizeof(short), 6);
    return endian_op(bodylen);
}

unsigned char message_base::check_code()
{
    unsigned char code;
    read_header_b((char*)&code, sizeof(unsigned char), 8);
    return endian_op(code);
}

void message_base::reset()
{
    readptr_ = c_header_size;
    size_ = c_header_size;
}

bool message_base::copy_b(const void* inbuf, int len)
{
    if(len > c_buffer_size)
    { return false; }
    reset();
    memcpy(buffer_, inbuf, len);
    size_ = len;
    return true;
}

void message_base::begin_b(short cmd, char ver, char subver)
{
    reset();
    const char flag[] = "GP";
		cmd = endian_op(cmd);
		ver = endian_op(ver);
		subver = endian_op(subver);
    write_header_b(flag, sizeof(char)*2, 0);
    write_header_b((char*)&cmd, sizeof(short), 2);
    write_header_b(&ver, sizeof(char), 4);
    write_header_b(&subver, sizeof(char), 5);
}

void message_base::end_b()
{
    short bodylen = static_cast<short>(size_ - c_header_size);
		bodylen = endian_op(bodylen);
    write_header_b((char*)&bodylen, sizeof(short), 6);
    unsigned char code = 0; // TODO do verification
		code = endian_op(code);
    write_header_b((char*)&code, sizeof(unsigned char), 8);
}

bool message_base::read_b(char* outbuf, int len)
{
    if((len + readptr_) > size_ || (len + readptr_) > c_buffer_size)
    { return false; }
    memcpy(outbuf, buffer_ + readptr_, len);
    readptr_ += len;
    return true;
}

bool message_base::read_del_b(char* outbuf, int len)
{
    if(!read_b(outbuf, len))
    { return false; }
    memmove(buffer_ + readptr_ - len, buffer_ + readptr_, size_ - readptr_);
    readptr_ -= len;
    size_ -= len;
    end_b();
    return true;
}

void message_base::read_undo_b(int len)
{
    readptr_ -= len;
}

char* message_base::read_bytes_b(int len)
{
    if((len + readptr_) > size_)
    { return NULL; }
    char* p = &buffer_[readptr_];
    readptr_ += len;
    return p;

}

bool message_base::write_b(const char* inbuf, int len)
{
    if((size_ < 0) || ((len + size_) > c_buffer_size))
    { return false; }
    memcpy(buffer_+size_, inbuf, len);
    size_ += len;
    return true;
}

bool message_base::write_front_b(const char* inbuf, int len)
{
    if((len + size_) > c_buffer_size)
    { return false; }
    memmove(buffer_ + c_header_size + len, buffer_ + c_header_size, size_ - c_header_size);
    memcpy(buffer_ + c_header_size, inbuf, len);
    size_ += len;
    end_b();
    return true;
}

bool message_base::write_zero_b()
{
    if((size_ + 1) > c_buffer_size)
    { return false; }
    memset(buffer_+size_, '\0', sizeof(char));
    ++size_;
    return true;
}

void message_base::read_header_b(char* outbuf, int len, int pos)
{
    if(pos > 0 || pos+len < c_header_size)
    { memcpy(outbuf, buffer_+pos, len); }
}

void message_base::write_header_b(const char* inbuf, int len, int pos)
{
    if(pos > 0 || pos+len < c_header_size)
    { memcpy(buffer_+pos, inbuf, len); }
}
