#include "myobj.h"

napi_ref MyObj::constructor;

void MyObj::Init(napi_env env, napi_value exports) {
	napi_status status;
	size_t pcount = 3;
	napi_property_descriptor properties[] = {
		{"value", 0, 0, GetValue, SetValue, 0, napi_default, 0},
		{"plusOne", 0, PlusOne, 0, 0, 0, napi_default, 0},
		{"multiply", 0, Multiply, 0, 0, 0, napi_default, 0}
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

	status = napi_set_named_property(env, exports, "MyObj", cons); // addon有个方法叫MyObj
	if (status != napi_ok) {
		napi_throw_error(env, NULL, "Unable to populate exports");
	}
}

void MyObj::Destructor(napi_env env, void* nativeObject, void* finalize_hint) {
	MyObj* obj = static_cast<MyObj*>(nativeObject);
	delete obj;
}

MyObj::MyObj(double value):value_(value),env_(nullptr),wrapper_(nullptr) {

}

MyObj::~MyObj() {
	napi_delete_reference(env_, wrapper_);
}

napi_value MyObj::New(napi_env env, napi_callback_info info) {
	napi_status status;
	
	napi_value is_constructor_val;
	// This API returns the new.target of the constructor call. 
	// If the current callback is not a constructor call, the result is nullptr.
	// napi_is_construct_call
	status = napi_get_new_target(env, info, &is_constructor_val);
	if (status != napi_ok) {
		printf("Failed to get new target\n");
		return nullptr;
	}
	bool is_constructor = (is_constructor_val != nullptr);

	if (is_constructor) {
		printf("call is_constructor\n");
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
	} else {
		printf("call no is_constructor\n");
		size_t argc = 1;
		napi_value args[1];
		status = napi_get_cb_info(env, info, &argc, args, NULL/* out:this */, NULL/* out:data */);
		if (status != napi_ok) {
			printf("Failed to get args\n");
			return nullptr;
		}

		napi_value argv[1] = {args[0]};

		napi_value cons;
		// If still valid, 
		// this API returns the napi_value representing the JavaScript Object associated with the napi_ref. 
		// Otherise, result will be NULL.
		status = napi_get_reference_value(env, constructor, &cons);
		if (status != napi_ok) {
			printf("Failed to get ref\n");
			return nullptr;
		}

		napi_value instance;
		// This method is used to instantiate a new JavaScript value using a given napi_value that represents the constructor for the object. 
		status = napi_new_instance(env, cons, argc, argv, &instance);
		if (status != napi_ok) {
			printf("Failed to new instance\n");
			return nullptr;
		}

		return instance;
	}
	return nullptr;
}

napi_value MyObj::GetValue(napi_env env, napi_callback_info info) {
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

	napi_value num;
	status = napi_create_double(env, obj->value_, &num);
	if (status != napi_ok) {
		printf("Failed to create number\n");
		return nullptr;
	}

	return num;
}

napi_value MyObj::SetValue(napi_env env, napi_callback_info info) {
	napi_status status;

	size_t argc = 1;
	napi_value args[1];
	napi_value jsthis;
	status = napi_get_cb_info(env, info, &argc, args, &jsthis, NULL/* out:data */);
	if (status != napi_ok) {
		printf("Failed to get args and this\n");
		return nullptr;
	}

	MyObj* obj;
	status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
	if (status != napi_ok) {
		printf("Failed to unwrap\n");
		return nullptr;
	}

	/*
		Returns napi_ok if the API succeeded. If a non-number napi_value is passed in it returns napi_number_expected.
		This API returns the C double primitive equivalent of the given JavaScript Number.
	*/
	status = napi_get_value_double(env, args[0], &obj->value_);
	if (status != napi_ok) {
		printf("Failed to get value double\n");
		return nullptr;
	}

	return nullptr;
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

napi_value MyObj::Multiply(napi_env env, napi_callback_info info) {
	napi_status status;

	size_t argc = 1;
	napi_value args[1];
	napi_value jsthis;
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

	double multiple = 1;
	if (valuetype != napi_undefined) {
		status = napi_get_value_double(env, args[0], &multiple);
		if (status != napi_ok) {
			printf("Failed to geet multiple\n"); 
			return nullptr;
		}
	}

	MyObj* obj;
	status = napi_unwrap(env, jsthis, reinterpret_cast<void**>(&obj));
	if (status != napi_ok) {
		printf("Failed to unwrap\n");
		return nullptr;
	}

	napi_value cons;
	status = napi_get_reference_value(env, constructor, &cons);
	if (status != napi_ok) {
		printf("Failed to get ref\n");
		return nullptr;
	}

	const int kArgCount = 1;
	napi_value argv[kArgCount];
	status = napi_create_double(env, obj->value_ * multiple, argv);
	if (status != napi_ok) {
		printf("Failed to create number\n");
		return nullptr;
	}

	napi_value instance;
	status = napi_new_instance(env, cons, kArgCount, argv, &instance);
	if (status != napi_ok) {
		printf("Failed to new instance\n");
		return nullptr;
	}

	return instance;
}
