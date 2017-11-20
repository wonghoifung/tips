#include <node.h>
#include "myobj.h"

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
	MyObj::Init(exports);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)