#ifndef CPARSERWRAPPER_HEADER
#define CPARSERWRAPPER_HEADER

#include <http_parser.h>

#define MAX_HEADERS 13
#define MAX_ELEMENT_SIZE 2048

struct requestmsg {
  char request_url[MAX_ELEMENT_SIZE];
  char headers[MAX_HEADERS][2][MAX_ELEMENT_SIZE];
  char body[MAX_ELEMENT_SIZE];
  size_t bodylen;
  enum { NONE=0, FIELD, VALUE } last_header_element;
  int num_headers;
  int body_is_final;
  unsigned int method; 
  unsigned short major; 
  unsigned short minor;
};

struct reqcontext {
  http_parser parser;
  struct http_parser_settings* settings;
  struct requestmsg msg;
  void* ud;
};
typedef struct reqcontext* reqcontext_ptr;

struct reqcontext* create_reqcontext();
void delete_reqcontext(struct reqcontext* ctx);
int cparse(struct reqcontext* ctx, const char* buf, int buflen, void* ud);
void build_headers_string(struct requestmsg* msg, char* output, size_t outlen, int delConnection);

#endif
