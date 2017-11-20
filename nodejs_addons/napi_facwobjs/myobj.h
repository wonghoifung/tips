#ifndef MYOBJ_HEADER
#define MYOBJ_HEADER

#include <node_api.h>
#include <stdio.h>

class MyObj {
public:
	static napi_status Init(napi_env env);
	static void Destructor(napi_env env, void* nativeObject, void* finalize_hint);
	static napi_status NewInstance(napi_env env, napi_value arg, napi_value* instance);
private:
	explicit MyObj(double value=0);
	~MyObj();
	static napi_value New(napi_env env, napi_callback_info info);
	static napi_value PlusOne(napi_env env, napi_callback_info info);
	static napi_ref constructor;
	double value_;
	napi_env env_;
	napi_ref wrapper_;
};

#endif
