#include "myobj.h"

napi_ref MyObj::constructor;

napi_status MyObj::Init(napi_env env) {
	napi_status status;
	size_t pcount = 1;
	napi_property_descriptor properties[] = {
		{"plusOne", 0, PlusOne, 0, 0, 0, napi_default, 0}
	};
	napi_value cons;
	// data: Optional data to be passed to the constructor callback as the data property of the callback info.
	status = napi_define_class(env, "MyObj", NAPI_AUTO_LENGTH, New, NULL/*data*/, pcount, properties, &cons);
	if (status != napi_ok) {
		napi_throw_error(env, NULL, "Failed to define class");
	}

	// cons 关联到静态constructor
	// This API create a new reference with the specified reference count to the Object passed in
	status = napi_create_reference(env, cons, 1, &constructor);
	if (status != napi_ok) {
		napi_throw_error(env, NULL, "Failed to create constructor ref");
	}

	return napi_ok;
}

void MyObj::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
	MyObj* obj = static_cast<MyObj*>(nativeObject);
	delete obj;
}

napi_status MyObj::NewInstance(napi_env env, napi_value arg, napi_value* instance) { // MyObj(...)
	printf("call NewInstance\n");
	napi_status status;

	size_t argc = 1;
	napi_value argv[1] = {arg};

	napi_value cons;
	status = napi_get_reference_value(env, constructor, &cons);
	if (status != napi_ok) {
		printf("Failed to get ref\n");
		return status;
	}

	status = napi_new_instance(env, cons, argc, argv, instance);
	if (status != napi_ok) {
		printf("Failed to new instance\n");
		return status;
	}

	return napi_ok;
}

MyObj::MyObj(double value):value_(value),env_(nullptr),wrapper_(nullptr) {

}

MyObj::~MyObj() {
	napi_delete_reference(env_, wrapper_);
}

napi_value MyObj::New(napi_env env, napi_callback_info info) { // new MyObj(...)
	printf("call New\n");
	napi_status status;

	size_t argc = 1;
	napi_value args[1];
	napi_value jsthis;
	// jsthis:
	//    Receives the JavaScript this argument for the call.
	status = napi_get_cb_info(env, info, &argc, args, &jsthis, NULL/* out:data */);
	if (status != napi_ok) {
		printf("Failed to get args and this\n");
		return nullptr;
	}

	napi_valuetype valuetype;
	status = napi_typeof(env, args[0], &valuetype);
	if (status != napi_ok) {
		printf("Failed to get type\n");
		return nullptr;
	}

	double value = 0;
	if (valuetype != napi_undefined) {
		status = napi_get_value_double(env, args[0], &value);
		if (status != napi_ok) {
			printf("Failed to geet value\n"); 
			return nullptr;
		}
	}

	MyObj* obj = new MyObj(value);
	obj->env_ = env;
	// jsthis:
	// 	The JavaScript object that will be the wrapper for the native object. 
	// 	This object must have been created from the prototype of a constructor that was created using napi_define_class()
	// obj:
	//  The native instance that will be wrapped in the JavaScript object.
	status = napi_wrap(env, jsthis, obj, MyObj::Destructor, nullptr, &obj->wrapper_);
	if (status != napi_ok) {
		printf("Failed to wrap\n");
		return nullptr;
	}

	return jsthis;
}

napi_value MyObj::PlusOne(napi_env env, napi_callback_info info) {
	napi_status status;

	size_t argc = 0;
	napi_value jsthis;
	status = napi_get_cb_info(env, info, &argc, NULL/* out:argv */, &jsthis, NULL/* out:data */);
	if (status != napi_ok) {
		printf("Failed to get this\n");
		return nullptr;
	}

	MyObj* obj;
	status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
	if (status != napi_ok) {
		printf("Failed to unwrap\n");
		return nullptr;
	}

	obj->value_ += 1;

	napi_value num;
	status = napi_create_double(env, obj->value_, &num);
	if (status != napi_ok) {
		printf("Failed to create number\n");
		return nullptr;
	}

	return num;
}
