#include <node_api.h>
#include <stdio.h>

napi_value say_hello(napi_env env, napi_callback_info info) {
	napi_value retval;
	printf("hello world\n");
	napi_create_int64(env, 1373, &retval);
	return retval;
}

napi_value init(napi_env env, napi_value exports) {
	napi_status status;
	napi_property_descriptor desc = {"hello_2", 0, say_hello, 0, 0, 0, napi_default, 0}; // addon.hello_2()
	// This method allows the efficient definition of multiple properties on a given object. 
	status = napi_define_properties(env, exports, 1, &desc);
	if (status != napi_ok) return NULL;
	return exports;
}

NAPI_MODULE(NAPI_HELLO2, init)