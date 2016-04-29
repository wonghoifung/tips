#include "inmessage.h"

int inmessage::read_int()
{
    int val(-1);
    message_base::read_b((char*)&val, sizeof(int));
    return endian_op(val);
}

int inmessage::read_int_and_repack()
{
    int val(-1);
    message_base::read_del_b((char*)&val, sizeof(int));
    return endian_op(val);
}

unsigned int inmessage::read_uint()
{
    unsigned int val(0);
    message_base::read_b((char*)&val, sizeof(unsigned int));
    return endian_op(val);
}

int64_t inmessage::read_int64()
{
    int64_t val(0);
    message_base::read_b((char*)&val, sizeof(int64_t));
    return endian_op(val);
}

uint64_t inmessage::read_uint64()
{
    uint64_t val(0);
    message_base::read_b((char*)&val, sizeof(uint64_t));
    return endian_op(val);
}

long inmessage::read_long()
{
    long val(0);
    message_base::read_b((char*)&val, sizeof(long));
    return endian_op(val);
}

unsigned long inmessage::read_ulong()
{
    unsigned long val(0);
    message_base::read_b((char*)&val, sizeof(unsigned long));
    return endian_op(val);
}

short inmessage::read_short()
{
    short val(-1);
    message_base::read_b((char*)&val, sizeof(short));
    return endian_op(val);
}

unsigned short inmessage::read_ushort()
{
    unsigned short val(0);
    message_base::read_b((char*)&val, sizeof(unsigned short));
    return endian_op(val);
}

char inmessage::read_char()
{
    char val(0);
    message_base::read_b((char*)&val, sizeof(char));
    return endian_op(val);
}

unsigned char inmessage::read_uchar()
{
    unsigned char val(0);
    message_base::read_b((char*)&val, sizeof(unsigned char));
    return endian_op(val);
}

char* inmessage::read_cstring()
{
    int len = read_int();
    if(len <= 0 || len >= c_buffer_size)
        return NULL;
    return message_base::read_bytes_b(len);
}

int inmessage::read_binary(char* outbuf, int maxlen)
{
    int len = read_int();
    if(len <= 0)
        return -1;
    if(len > maxlen)
    {
        message_base::read_undo_b(sizeof(int));
        return -1;
    }
    if(message_base::read_b(outbuf, len))
        return len ;
    return 0;
}

bool inmessage::copy(const void* inbuf, int len)
{
    return message_base::copy_b(inbuf, len);
}

void inmessage::begin(short cmd, char ver, char subver)
{
    message_base::begin_b(cmd, ver, subver);
}

void inmessage::end()
{
    message_base::end_b();
}
