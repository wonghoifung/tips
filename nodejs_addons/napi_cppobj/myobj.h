#ifndef MYOBJ_HEADER
#define MYOBJ_HEADER

#include <node_api.h>
#include <stdio.h>

class MyObj {
public:
	static void Init(napi_env env, napi_value exports);
	static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
private:
	explicit MyObj(double value=0);
	~MyObj();
	static napi_value New(napi_env env, napi_callback_info info);
	static napi_value GetValue(napi_env env, napi_callback_info info);
	static napi_value SetValue(napi_env env, napi_callback_info info);
	static napi_value PlusOne(napi_env env, napi_callback_info info);
	static napi_value Multiply(napi_env env, napi_callback_info info);
	static napi_ref constructor;
	double value_;
	napi_env env_;
	napi_ref wrapper_;
};

#endif
