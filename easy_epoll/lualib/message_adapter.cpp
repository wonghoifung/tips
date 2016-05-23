#include "message_adapter.h"

mfunc_item<inmessage_adapter> inmessage_adapter::mfuncs[8] = {
    {"command", &inmessage_adapter::command},
    {"read_int", &inmessage_adapter::read_int},
    {"read_int_and_repack", &inmessage_adapter::read_int_and_repack},
    {"read_short", &inmessage_adapter::read_short},
    {"read_char", &inmessage_adapter::read_char},
    {"read_cstring", &inmessage_adapter::read_cstring},
    {"copy", &inmessage_adapter::copy},
    {NULL, NULL}
};

mfunc_item<outmessage_adapter> outmessage_adapter::mfuncs[11] = {
    {"buffer", &outmessage_adapter::buffer},
    {"size", &outmessage_adapter::size},
    {"write_int", &outmessage_adapter::write_int},
    {"write_char", &outmessage_adapter::write_char},
    {"write_short", &outmessage_adapter::write_short},
    {"write_front_int", &outmessage_adapter::write_front_int},
    {"write_front_char", &outmessage_adapter::write_front_char},
    {"write_cstring", &outmessage_adapter::write_cstring},
    {"begin", &outmessage_adapter::begin},
    {"end", &outmessage_adapter::end},
    {NULL, NULL}
};
