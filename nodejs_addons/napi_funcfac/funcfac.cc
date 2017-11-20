#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

napi_value Method(napi_env env, napi_callback_info info) {
	napi_status status;
	napi_value s;
	status = napi_create_string_utf8(env, "hello world", NAPI_AUTO_LENGTH, &s);
	if (status != napi_ok) return nullptr;
	return s;
}

napi_value CreateMethod(napi_env env, napi_callback_info info) {
	napi_value method;
	napi_status status;
	status = napi_create_function(env, NULL/*function name*/, 0, Method, NULL, &method);
	if (status != napi_ok) return nullptr;
	return method;
}

napi_value init(napi_env env, napi_value exports) {
	napi_value method;
	napi_status status;
	status = napi_create_function(env, "exports", 0, CreateMethod, NULL, &method); // addon就是一个仿函数
	if (status != napi_ok) return nullptr;
	return method;
}

NAPI_MODULE(FUNCFAC, init)