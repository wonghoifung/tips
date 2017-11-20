#include "myobj.h"

napi_value CreateObject(napi_env env, napi_callback_info info) {
	napi_status status;

	size_t argc = 1;
	napi_value args[1];
	status = napi_get_cb_info(env, info, &argc, args, NULL/* out:this */, NULL/* out:data */);
	if (status != napi_ok) {
		printf("Failed to get args\n");
		return nullptr;
	}

	napi_value instance;
	status = MyObj::NewInstance(env, args[0], &instance);
	if (status != napi_ok) {
		printf("Failed to NewInstance\n");
		return nullptr;
	}

	return instance;
}

napi_value Init(napi_env env, napi_value exports) {
	napi_value method;

	napi_status status;
	status = MyObj::Init(env);
	if (status != napi_ok) {
		printf("Failed to Init MyObj\n");
		return nullptr;
	}

	status = napi_create_function(env, "exports", 0, CreateObject, NULL, &method); // addon就是一个仿函数
	if (status != napi_ok) {
		printf("Failed to create function\n");
		return nullptr;
	}
	return method;
}

NAPI_MODULE(xxxxxxxx, Init)