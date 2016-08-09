#include <stdio.h>

int main() {
  int a[10];
  void* p = 1024;
  void* q = 0;
  printf("sizeof int: %d, sizeof void*: %d\n", sizeof(int), sizeof(int));
  printf("p-q=%d\n",p-q);
  printf("address: %p, %p, %p\n", &a[0], a, &a);
  printf("address: %p, %p\n", a+1, &a+1);
}

