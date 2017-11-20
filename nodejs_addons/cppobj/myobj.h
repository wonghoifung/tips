#ifndef MYOBJ_HEADER
#define MYOBJ_HEADER

#include <node.h>
#include <node_object_wrap.h>

class MyObj : public node::ObjectWrap { // 要输出的c++ class都要继承这个东西
public:
	static void Init(v8::Local<v8::Object> exports);
private:
	explicit MyObj(double value=0);
	~MyObj();
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor; // 构造函数
	double value_;
};

#endif
