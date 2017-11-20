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

// void MyObj::Init(v8::Local<v8::Object> exports) {
// 	Isolate* isolate = exports->GetIsolate();

// 	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
// 	tpl->SetClassName(String::NewFromUtf8(isolate, "MyObj"));
// 	tpl->InstanceTemplate()->SetInternalFieldCount(1);

// 	NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

// 	constructor.Reset(isolate, tpl->GetFunction());
// 	exports->Set(String::NewFromUtf8(isolate, "MyObj"), tpl->GetFunction());
// }

void MyObj::Init(v8::Isolate* isolate) {
	// prepare constructor tempalte
	Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
	tpl->SetClassName(String::NewFromUtf8(isolate, "MyObj"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// prototype
	NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);

	constructor.Reset(isolate, tpl->GetFunction());
}

void MyObj::NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	const unsigned argc = 1;
	Local<Value> argv[argc] = {args[0]};
	Local<Context> context = isolate->GetCurrentContext();
	Local<Function> cons = Local<Function>::New(isolate, constructor);
	Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
	args.GetReturnValue().Set(result);
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
		obj->Wrap(args.This());
		args.GetReturnValue().Set(args.This());
	} else {
		// MyObj(...)
		const int argc = 1;
		Local<Value> argv[argc] = {args[0]};
		Local<Context> context = isolate->GetCurrentContext();
		Local<Function> cons = Local<Function>::New(isolate, constructor);
		Local<Object> result = cons->NewInstance(context, argc, argv).ToLocalChecked();
		args.GetReturnValue().Set(result);
	}
}

void MyObj::PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args) {
	Isolate* isolate = args.GetIsolate();

	MyObj* obj = ObjectWrap::Unwrap<MyObj>(args.Holder());
	obj->value_ += 1;

	args.GetReturnValue().Set(Number::New(isolate, obj->value_));
}
