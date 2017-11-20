#include <node_api.h>
#include <stdio.h>

napi_value MyFunction(napi_env env, napi_callback_info info) {
  napi_status status;
  size_t argc = 2;
  int number1 = 0;
  int number2 = 0;
  napi_value argv[2];
  // This method is used within a callback function to retrieve details about the call like the arguments 
  // and the this pointer from a given callback info.
  status = napi_get_cb_info(env, info, &argc/* in/out */, argv, NULL/* out:this */, NULL/* out:data */);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Failed to parse arguments");
  }

  status = napi_get_value_int32(env, argv[0], &number1);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid number1 was passed as argument");
  }

  status = napi_get_value_int32(env, argv[1], &number2);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Invalid number2 was passed as argument");
  }


  napi_value myNumber;
  int number = number1 + number2;
  printf("number1:%d number2:%d number:%d\n", number1, number2, number);
  // This API is used to convert from the C int32_t type to the JavaScript Number type.
  status = napi_create_int32(env, number, &myNumber);

  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to create return value");
  }

  return myNumber;
}

// napi_env: 就是context，表示js虚拟机， Caching the napi_env for the purpose of general reuse is not allowed
// napi_value: This is an opaque pointer that is used to represent a JavaScript value.
napi_value Init(napi_env env, napi_value exports) {
  // napi_status: Integral status code indicating the success or failure of a N-API call. 是一个enum类型
  napi_status status;
  napi_value fn;

  // It's used to wrap native functions so that they can be invoked from JavaScript.
  status = napi_create_function(env, NULL, 0, MyFunction, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }

  status = napi_set_named_property(env, exports, "add", fn); // addon的一个成员方法叫add
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }

  return exports;
}

NAPI_MODULE(ARGUMENTS, Init)