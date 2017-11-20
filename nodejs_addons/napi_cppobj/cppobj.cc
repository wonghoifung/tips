#include <node_api.h>
#include <stdio.h>
#include "myobj.h"

napi_value Init(napi_env env, napi_value exports) {
  MyObj::Init(env, exports);
  return exports;
}

NAPI_MODULE(CPPOBJ, Init)