#include <node_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

napi_value Method(napi_env env, napi_callback_info info) {
	napi_status status = napi_generic_failure;

	size_t argc = 1;
	napi_value argv[1];
	// This method is used within a callback function to retrieve details about the call like the arguments 
	// and the this pointer from a given callback info.
	status = napi_get_cb_info(env, info, &argc, argv, NULL, NULL);
	if (status != napi_ok) {
		printf("cannot get cb info\n");
		return nullptr;
	}

	napi_value s;
	status = napi_create_string_utf8(env, "hello world~~~~~~", NAPI_AUTO_LENGTH, &s);
	if (status != napi_ok) {
		printf("cannot create string\n");
		return nullptr;
	}

	napi_value* argv_ = &s;
	size_t argc_ = 1;

	napi_value global;
	status = napi_get_global(env, &global); // This API returns the global Object.

	napi_value return_val;
	// This method allows a JavaScript function object to be called from a native add-on. 
	status = napi_call_function(env, global/* this */, argv[0], argc_, argv_, &return_val);
	if (status != napi_ok) {
		// napi_throw_error(env, NULL, "call failure");
		printf("cannot call function : %d\n", status);
		return nullptr;
	}

	return nullptr;
}

napi_value init(napi_env env, napi_value exports) {
	napi_value method;
	napi_status status;
	// It's used to wrap native functions so that they can be invoked from JavaScript.
	//                           js vm,fname,fname len,fpointer,data,result 
	status = napi_create_function(env, "exports", 0, Method, NULL, &method); // addon就是仿函数
	if (status != napi_ok) return nullptr;
	return method;
}

NAPI_MODULE(CALLBACKS, init) // 注册