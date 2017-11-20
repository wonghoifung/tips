#ifndef MYOBJ_HEADER
#define MYOBJ_HEADER

#include <node.h>
#include <node_object_wrap.h>

class MyObj : public node::ObjectWrap {
public:
	// static void Init(v8::Local<v8::Object> exports);
	static void Init(v8::Isolate* isolate);
	static void NewInstance(const v8::FunctionCallbackInfo<v8::Value>& args);
	inline double value() const {return value_;}
private:
	explicit MyObj(double value=0);
	~MyObj();
	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void PlusOne(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor;
	double value_;
};

#endif
