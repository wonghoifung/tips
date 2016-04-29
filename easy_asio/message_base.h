#ifndef message_base_header
#define message_base_header

#include <memory>
#include <string>
#include <cstdarg>
#include <string.h>
#include "endian_op.h"

enum { c_default_version = 1, c_default_subversion = 1 };
enum { c_header_size = 9, c_buffer_size = 1024 * 12 };

class message_base
{
public:
    message_base(){}
    virtual ~message_base(){}

    char*       buffer() { return buffer_; }
    const char* cbuffer() const	{ return buffer_; }
    int         size() const { return size_; }

    short         command();
    char          version();
    char          subversion();
    short         body_length();
    unsigned char check_code();
    void          reset();

protected:
    bool  copy_b(const void* inbuf, int len);
    void  begin_b(short cmd, char ver, char subver);
    void  end_b();

    bool  read_b(char* outbuf, int len);
    bool  read_del_b(char* outbuf, int len);
    void  read_undo_b(int len);
    char* read_bytes_b(int len);

    bool  write_b(const char* inbuf, int len);
    bool  write_front_b(const char* inbuf, int len);
    bool  write_zero_b();

    void  read_header_b(char* outbuf, int len, int pos);
    void  write_header_b(const char* inbuf, int len, int pos);

private:
    char buffer_[c_buffer_size];
    int size_ ;
    int readptr_;
};

#endif
