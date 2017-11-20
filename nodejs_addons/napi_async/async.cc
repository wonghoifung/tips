#include <node_api.h>
#include <stdio.h>
#include <unistd.h>

napi_value bye_sync(napi_env env, napi_callback_info info) {
	napi_value retval;
	printf("hello sync\n");
	sleep(1);
	printf("bye sync\n");
	napi_create_int64(env, 8888, &retval);
	return retval;
}

void bye_async_execute(napi_env env, void* data) {
	printf("hello async\n");
	sleep(1);
	printf("bye async\n");
}

void bye_async_complete(napi_env env, napi_status status, void* data) {
	printf("hello completed async\n");
}

napi_value bye_async(napi_env env, napi_callback_info info) {
	napi_value retval;
	napi_async_work work;
	napi_value async_resource_name;

	napi_create_string_utf8(env, "bye:sleep", -1, &async_resource_name);
	// This API allocates a work object that is used to execute logic asynchronously. 
	// It should be freed using napi_delete_async_work once the work is no longer required.
	napi_create_async_work(env, NULL, async_resource_name, bye_async_execute, bye_async_complete, NULL, &work);
	// This API requests that the previously allocated work be scheduled for execution.
	napi_queue_async_work(env, work);

	napi_create_int64(env, 1373, &retval);
	return retval;
}

napi_value init(napi_env env, napi_value exports) {
	napi_status status;
	napi_property_descriptor desc[] = {
		{"byeSync", 0, bye_sync, 0, 0, 0, napi_default, 0}, // addon.byeSync()
		{"byeASync", 0, bye_async, 0, 0, 0, napi_default, 0}// addon.byeASync()
	};
	// 快捷定义addon方法
	status = napi_define_properties(env, exports, 2, desc);
	if (status != napi_ok) return NULL;
	return exports;
}

NAPI_MODULE(xxxxxxx,init)