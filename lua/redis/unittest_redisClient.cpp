#include "redisClient.h"
#include <stdio.h>

static void check_bool(const char* tag, bool b) {
	if (b) {
		printf("%10s ok"\n);
	} else {
		printf("%10s failure\n");
	}
}

int main() {
	redisClient rc("127.0.0.1", 6379);
	redisClient* prc = &rc;
	check_bool(prc->connect());
	check_bool(prc->ping());

	return 0;
}
