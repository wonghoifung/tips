#include <node.h>

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void MyFunction(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();
	args.GetReturnValue().Set(String::NewFromUtf8(isolate,"hello world"));
}

void CreateFunction(const FunctionCallbackInfo<Value>& args) {
	Isolate* isolate = args.GetIsolate();

	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, MyFunction);
	Local<Function> fn = tpl->GetFunction();

	fn->SetName(String::NewFromUtf8(isolate, "theFunction"));

	args.GetReturnValue().Set(fn); // 返回一个函数
}

void Init(Local<Object> exports, Local<Object> module) {
	NODE_SET_METHOD(module, "exports", CreateFunction); // addon就是一个仿函数
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)