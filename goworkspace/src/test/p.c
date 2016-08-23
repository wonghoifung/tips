#include "p.h"
#include <stdio.h>

extern void Gp(int a);

void p(char* s) {
	printf("C: %s\n", s);
	Gp(888);
}
