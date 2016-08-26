#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cparserwrapper.h"

static const char data[] = 
         "POST /post_identity_body_world?q=search#hey HTTP/1.1\r\n"
         "Accept: */*\r\n"
         "Connection: keep-alive\r\n"
         "Transfer-Encoding: identity\r\n"
         "Content-Length: 5\r\n"
         "\r\n"
         "World";
static const size_t data_len = sizeof(data) - 1;

int 
main() {
  struct reqcontext* ctx = create_reqcontext();

  for (size_t i = 0; i<data_len; ++i) {
    int ret = cparse(ctx, &(data[i]), 1, NULL);
    if (ret != 0) return ret;
  }

  printf("=============\n");
  printf("%s\n", data);
  printf("=============\n");
  char headers[8192] = {0};
  build_headers_string(&ctx->msg, headers, sizeof(headers), 0);
  printf("is headers equal: %d\n", strncmp(headers, data, strlen(headers))==0);

  delete_reqcontext(ctx);
}
