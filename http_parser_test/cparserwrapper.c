#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cparserwrapper.h"

/* strnlen() is a POSIX.2008 addition. Can't rely on it being available so
 * define it ourselves.
 */
static size_t
strnlen(const char *s, size_t maxlen)
{
  const char *p;

  p = memchr(s, '\0', maxlen);
  if (p == NULL)
    return maxlen;

  return p - s;
}

static size_t
strlncat(char *dst, size_t len, const char *src, size_t n)
{
  size_t slen;
  size_t dlen;
  size_t rlen;
  size_t ncpy;

  slen = strnlen(src, n);
  dlen = strnlen(dst, len);

  if (dlen < len) {
    rlen = len - dlen;
    ncpy = slen < rlen ? slen : (rlen - 1);
    memcpy(dst + dlen, src, ncpy);
    dst[dlen + ncpy] = '\0';
  }

  assert(len > slen + dlen);
  return slen + dlen;
}

/* output should be intialized to array filling with 0 first */
void 
build_headers_string(struct requestmsg* msg, char* output, size_t outlen, int delConnection) {
  snprintf(output, outlen, "%s %s HTTP/%d.%d\r\n",
    http_method_str((enum http_method)msg->method), msg->request_url, msg->major, msg->minor);
  size_t len = strnlen(output, outlen);
  if (len == outlen) {
    printf("not enough output space for headers, line %d\n", __LINE__);
    return;
  }
  for (int i=0; i<msg->num_headers; ++i) {
    if (delConnection && strcmp(msg->headers[i][0], "Connection") == 0)
      continue;
    snprintf(output + len, outlen - len, "%s: %s\r\n", msg->headers[i][0], msg->headers[i][1]);
    len = strnlen(output, outlen);
    if (len == outlen) {
      printf("not enough output space for headers, line %d\n", __LINE__);
      return;
    }
  }
  char* emptyline = "\r\n";
  strlncat(output, outlen, emptyline, strlen(emptyline));
}

static int 
c_on_message_begin(http_parser* p) {
	return 0; 
}

static int 
c_on_headers_complete(http_parser* p) {
  struct reqcontext* ctx = (struct reqcontext*)(p->data);
  struct requestmsg* m = &ctx->msg;
  m->method = p->method;
  m->major = p->http_major;
  m->minor = p->http_minor;
  // char headers[8192] = {0};
  // build_headers_string(m, headers, sizeof(headers), 1);
  // printf("-----------\n");
  // printf("%s\n", headers);
  // printf("-----------\n");
	return 0;
}

static int 
c_on_message_complete(http_parser* p) {
  struct reqcontext* ctx = (struct reqcontext*)(p->data);
  struct requestmsg* m = &ctx->msg;
  // printf("body_is_final: %d\n", m->body_is_final);
  // printf("-----------\n");
  // printf("%s\n", m->body);
  // printf("-----------\n");
  // printf("bodylen: %d\n", m->bodylen);
  // printf("-----------\n");
	return 0;
}

static int 
c_on_header_field(http_parser* p, const char* at, size_t length) {
  struct reqcontext* ctx = (struct reqcontext*)(p->data);
  struct requestmsg* m = &ctx->msg;
  if (m->last_header_element != FIELD)
    m->num_headers++;
  strlncat(m->headers[m->num_headers-1][0], sizeof(m->headers[m->num_headers-1][0]), at, length);
  m->last_header_element = FIELD;
	return 0; 
}

static int 
c_on_header_value(http_parser* p, const char* at, size_t length) {
  struct reqcontext* ctx = (struct reqcontext*)(p->data);
  struct requestmsg* m = &ctx->msg;
  strlncat(m->headers[m->num_headers-1][1], sizeof(m->headers[m->num_headers-1][1]), at, length);
  m->last_header_element = VALUE;
	return 0; 
}

static int 
c_on_url(http_parser* p, const char* at, size_t length) {
  struct reqcontext* ctx = (struct reqcontext*)(p->data);
  struct requestmsg* msg = &ctx->msg;
  strlncat(msg->request_url, sizeof(msg->request_url), at, length);
	return 0;
}

static int 
c_on_status(http_parser* p, const char* at, size_t length) {
	(void)p; (void)at; (void)length;
	return 0;
}

static int 
c_on_body(http_parser* p, const char* at, size_t length) {
  struct reqcontext* ctx = (struct reqcontext*)(p->data);
  struct requestmsg* msg = &ctx->msg;
  strlncat(msg->body, sizeof(msg->body), at, length);
  msg->bodylen += length;

  if (msg->body_is_final) {
    fprintf(stderr, "\n\n *** error http_body_is_final() should return 1 "
                    "on last on_body callback call "
                    "but it doesn't! ***\n\n");
    assert(0);
    abort();
  }
  msg->body_is_final = http_body_is_final(p);

	return 0;
}

static struct http_parser_settings settings = {
  .on_message_begin = c_on_message_begin,
  .on_headers_complete = c_on_headers_complete,
  .on_message_complete = c_on_message_complete,
  .on_header_field = c_on_header_field,
  .on_header_value = c_on_header_value,
  .on_url = c_on_url,
  .on_status = c_on_status,
  .on_body = c_on_body
};

struct reqcontext* 
create_reqcontext() {
  struct reqcontext* ctx = (struct reqcontext*)malloc(sizeof(struct reqcontext));
  http_parser_init(&(ctx->parser), HTTP_REQUEST);
  memset((char*)&(ctx->msg), 0, sizeof(struct requestmsg));
  ctx->parser.data = ctx;
  ctx->settings = &settings;
  ctx->ud = NULL;
}

void 
delete_reqcontext(struct reqcontext* ctx) {
  free(ctx);
}

int
cparse(struct reqcontext* ctx, const char* buf, int buflen, void* ud) {
  ctx->ud = ud;
  size_t nparsed = http_parser_execute(&ctx->parser, ctx->settings, buf, buflen);
  if (nparsed != buflen) {
    printf("error: %s (%s)\n", 
        http_errno_description(HTTP_PARSER_ERRNO(&ctx->parser)), 
        http_errno_name(HTTP_PARSER_ERRNO(&ctx->parser)));
    return -1;
  }
  return 0;
}

