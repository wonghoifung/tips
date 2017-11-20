#include <node.h>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::String;
using v8::Value;

void Add(const FunctionCallbackInfo<Value>& args) {
	/* isolate代表一个v8 engine 实例。
	   各个isolate之间的状态完全独立，
	   位于一个isolate中的object无法在另外一个isolate中被使用 */
	Isolate* isolate = args.GetIsolate();

	/* context代表js虚拟机
	   一个isolate可以有多个context，且可在这些context中切换
	   Context::Scope scopeB(contextB) 表示进入contextB
	*/

	/* 在v8 engine中通过handle访问存在于heap上的JS object。
	   handle有不少种类，包括Local, Persistent, Eternal等。
	   在v8中有一个handle stack用于管理这些handles
	*/

	if (args.Length() < 2) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return;
	}

	if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
		isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong arguments")));
		return;
	}

	double value = args[0]->NumberValue() + args[1]->NumberValue();
	Local<Number> num = Number::New(isolate, value);

	args.GetReturnValue().Set(num);
}

void Init(Local<Object> exports) {
	NODE_SET_METHOD(exports, "add", Add); // addon的一个成员方法叫add
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
