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

napi_value Add(napi_env env, napi_callback_info info) {
	napi_status status;

	size_t argc = 2;
	napi_value args[2];
	status = napi_get_cb_info(env, info, &argc, args, NULL/* out:this */, NULL/* out:data */);
	if (status != napi_ok) {
		printf("Failed to get args\n");
		return nullptr;
	}

	MyObj* obj1;
	status = napi_unwrap(env, args[0], reinterpret_cast<void**>(&obj1));
	if (status != napi_ok) {
		printf("Failed to unwrap\n");
		return nullptr;
	}

	MyObj* obj2;
	status = napi_unwrap(env, args[1], reinterpret_cast<void**>(&obj2));
	if (status != napi_ok) {
		printf("Failed to unwrap\n");
		return nullptr;
	}

	napi_value sum;
	status = napi_create_double(env, obj1->Val() + obj2->Val(), &sum);
	if (status != napi_ok) {
		printf("Failed to create double\n");
		return nullptr;
	}

	return sum;
}

napi_value Init(napi_env env, napi_value exports) {
	napi_status status;
	status = MyObj::Init(env);
	if (status != napi_ok) {
		printf("Failed to Init MyObj\n");
		return nullptr;
	}

	napi_property_descriptor desc[] = {
		{"createObject", 0, CreateObject, 0, 0, 0, napi_default, 0},
		{"add", 0, Add, 0, 0, 0, napi_default, 0}
	};

	status = napi_define_properties(env, exports, sizeof(desc)/sizeof(*desc), desc);
	if (status != napi_ok) {
		printf("Failed to define properties\n");
		return nullptr;
	}

	return exports;
}

NAPI_MODULE(xxxxxxxx, Init)