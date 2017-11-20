#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <node.h>

using node::AtExit;
using v8::HandleScope;
using v8::Isolate;
using v8::Local;
using v8::Object;

static char cookie[] = "yum yum";
static int at_exit_cb1_called = 0;
static int at_exit_cb2_called = 0;

static void at_exit_cb1(void* arg) {
	printf("at_exit_cb1\n");
	Isolate* isolate = static_cast<Isolate*>(arg);
	// HandleScope：可以把它想象成是多个Handle的一个容器。
	// 在v8中有一个handle stack用于管理这些handles
	HandleScope scope(isolate);
	Local<Object> obj = Object::New(isolate);
	assert(!obj.IsEmpty());
	assert(obj->IsObject());
	at_exit_cb1_called += 1;
}

static void at_exit_cb2(void* arg) {
	printf("at_exit_cb2\n");
	assert(arg == static_cast<void*>(cookie));
	at_exit_cb2_called += 1;
}

static void sanity_check(void*) {
	printf("sanity_check\n");
	assert(at_exit_cb1_called == 1);
	assert(at_exit_cb2_called == 2);
}

void init(Local<Object> exports) {
	// AtExit takes two parameters: a pointer to a callback function to run at exit, 
	// and a pointer to untyped context data to be passed to that callback.
	AtExit(at_exit_cb2, cookie);
	AtExit(at_exit_cb2, cookie);
	AtExit(at_exit_cb1, exports->GetIsolate());
	AtExit(sanity_check);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, init)