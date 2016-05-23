#include "SimpleServerAdapter.h"

mfunc_item<SimpleServerAdapter> SimpleServerAdapter::mfuncs[4] = {
    {"init_server", &SimpleServerAdapter::init_server},
    {"init", &SimpleServerAdapter::init},
    {"run", &SimpleServerAdapter::run},
    {NULL, NULL}
};
