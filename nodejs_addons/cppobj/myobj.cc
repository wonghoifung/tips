#include "myobj.h"

using v8::Context;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

Persistent<Function> MyObj::constructor;

void MyObj::Init(v8::Local<v8::Object> exports) {
	Isolate* isolate = exports->GetIsolate();

	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "MyObj"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne); // 设置成员函数

	constructor.Reset(isolate, tpl->GetFunction()); // 设置构造函数
	exports->Set(String::NewFromUtf8(isolate, "MyObj"), tpl->GetFunction()); // new addon.MyObj(...) , addon.MyObj(...)
}

MyObj::MyObj(double value):value_(value) {

}

MyObj::~MyObj() {

}

void MyObj::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	if (args.IsConstructCall()) {
		// new MyObj(...)
		double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
		MyObj* obj = new MyObj(value);
		obj->Wrap(args.This()); // 打包成js对象
		args.GetReturnValue().Set(args.This()); // 返回js对象
	} else {
		// MyObj(...)
		const int argc = 1;
		Local<Value> argv[argc] = {args[0]};
		Local<Context> context = isolate->GetCurrentContext(); // 取出当前js vm
		Local<Function> cons = Local<Function>::New(isolate, constructor); // 生成构造函数
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void MyObj::PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	MyObj* obj = ObjectWrap::Unwrap<MyObj>(args.Holder()); // 取出c++对象指针
	obj->value_ += 1;

	args.GetReturnValue().Set(Number::New(isolate, obj->value_)); // 返回最新值
}
