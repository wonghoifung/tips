#include <node_api.h>

napi_value HelloMethod(napi_env env, napi_callback_info info) {
	napi_value world;
	napi_create_string_utf8(env, "world", 5, &world);
	return world;
}

napi_value Init(napi_env env, napi_value exports) {
	napi_property_descriptor desc = {"hello", 0, HelloMethod, 0, 0, 0, napi_default, 0}; // addon.hello()
	// This method allows the efficient definition of multiple properties on a given object. 
	napi_define_properties(env, exports, 1, &desc);
	return napi_value();
}

NAPI_MODULE(hello, Init)