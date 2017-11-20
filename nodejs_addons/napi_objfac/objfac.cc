#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

napi_value Method(napi_env env, napi_callback_info info) {
	napi_status status = napi_generic_failure;

	napi_value obj;
	status = napi_create_object(env, &obj);
	if (status != napi_ok) return nullptr;

	size_t argc = 1;
	napi_value argv[1];
	status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
	if (status != napi_ok) {
		// napi_throw_error(env, NULL, "Failed to parse arguments");
		return nullptr;
	}

	// This method is equivalent to calling napi_set_property with a napi_value created from the string passed in as utf8Name
	status = napi_set_named_property(env, obj, "msg", argv[0]);
	if (status != napi_ok) return nullptr;

	return obj;
}

napi_value init(napi_env env, napi_value exports) {
	napi_value method;
	napi_status status;
	status = napi_create_function(env, "exports", 0, Method, NULL, &method); // addon就是一个仿函数
	if (status != napi_ok) return nullptr;
	return method;
}

NAPI_MODULE(OBJFAC, init)