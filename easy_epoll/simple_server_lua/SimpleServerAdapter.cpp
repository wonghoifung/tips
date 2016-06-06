#include "SimpleServerAdapter.h"

mfunc_item<SimpleServerAdapter> SimpleServerAdapter::mfuncs[2] = {
    {"init", &SimpleServerAdapter::init},
    {NULL, NULL}
};
