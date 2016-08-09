#include <stdio.h>
#if 0
class base 
{
public:
  base() {printf("base construct\n");}
  void virtual func() {printf("base destruct\n");}
  ~base() {func();}
};

class derived:public base
{
public:
  derived() {printf("derived construct\n");}
  void virtual func() {printf("derived destruct\n");}
  ~derived() {func();}
};

int main() {
  base father;
  derived son;
  return 0;
}
#endif

#if 0
class base
{
public:
  base() {printf("base construct\n");}
  ~base() {printf("base destruct\n");}
};

class derived:public base
{
public:
  derived() {printf("derived construct\n");}
  ~derived() {printf("derived destruct\n");}
};

int main() {
  base* pBase = new derived;
  delete pBase;
  return 0;
}
#endif

class base
{
public:
  base() {printf("construct\n");}
  base(const base& obj) {printf("construct from another\n");}
  base& operator=(const base& obj) {printf("assignment\n");return *this;}
};

int main() {
  base one;
  base two = one;
  two = one;
  return 0;
}

