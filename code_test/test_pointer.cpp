#include <stdio.h>
#include <stdlib.h>

void func(char str[100]) {
  printf("%d\n",sizeof(str)==8);
}

struct X {int a; char b; int c;};

struct Y {int a; char b; int c; char d;};

struct Z {int i; int a[0];};

class C {
public:
  int a;
  char b;
  static int c;
};

int main() {
  int a[10];
  void* p = (void*)1024;
  void* q = 0;
  printf("sizeof int: %d, sizeof void*: %d\n", sizeof(int), sizeof(void*));
  //printf("p-q=%d\n",p-q);
  printf("address: %p, %p, %p\n", &a[0], a, &a);
  printf("address: %p, %p\n", a+1, &a+1);

  {
    char str[] = "Hello";
    char* p = str;
    int n = 10;
    printf("%d\n",sizeof(str)==6);
    printf("%d\n",sizeof(p)==8);
    printf("%d\n",sizeof(n)==4);
    {
      char strr[100] = {0};
      func(strr);
    }
  }

  {
    void* p = malloc(100);
    printf("%d\n", sizeof(p)==8);
    printf("%d\n", sizeof('A')==1); // different from c
    printf("%d\n", sizeof(struct X)==12);
    printf("%d\n", sizeof(struct Y)==16);
    printf("%d\n", sizeof(struct Z)==4);
    printf("%d\n", sizeof(C)==8);
  }
}

