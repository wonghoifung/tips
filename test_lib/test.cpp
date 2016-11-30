#include "dy.h"
#include "st.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
  void* handle;
  void (*yy)();
  char* error;

  handle = dlopen("libdy2.so", RTLD_LAZY);
  if (!handle) 
  {
    fprintf(stderr, "%s\n", dlerror());
    exit(EXIT_FAILURE);
  }
  dlerror();

  *(void**)(&yy) = dlsym(handle, "yy");

  if ((error = dlerror()) != NULL) 
  {
    fprintf(stderr, "%s\n", error);
    exit(EXIT_FAILURE);
  }

  while (1) 
  {
    sleep(1);
    pp();
    ss();
    yy();
  }

  dlclose(handle);
}

