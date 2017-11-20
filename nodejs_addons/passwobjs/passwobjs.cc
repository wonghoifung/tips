#include <node.h>
#include <node_object_wrap.h>
#include "myobj.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void CreateObject(const FunctionCallbackInfo<Value>& args) {
	MyObj::NewInstance(args);
}

void Add(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	MyObj* obj1 = node::ObjectWrap::Unwrap<MyObj>(args[0]->ToObject());
	MyObj* obj2 = node::ObjectWrap::Unwrap<MyObj>(args[1]->ToObject());

	double sum = obj1->value() + obj2->value();

	args.GetReturnValue().Set(Number::New(isolate, sum));
}

void InitAll(Local<Object> exports) {
	MyObj::Init(exports->GetIsolate());

	NODE_SET_METHOD(exports, "createObject", CreateObject);
	NODE_SET_METHOD(exports, "add", Add);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)