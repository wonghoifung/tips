#include <node.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void CreateObject(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	Local<Object> obj = Object::New(isolate);
	obj->Set(String::NewFromUtf8(isolate,"msg"), args[0]->ToString());

	args.GetReturnValue().Set(obj); // 返回一个对象
}

void Init(Local<Object> exports, Local<Object> module) {
	NODE_SET_METHOD(module, "exports", CreateObject); // addon就是一个仿函数
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)