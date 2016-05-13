#ifndef LOG_HEADER
#define LOG_HEADER

#include <sys/cdefs.h>
#include <stdio.h>

__BEGIN_DECLS

#define DEFAULT_FILE_NUM    3
#define DEFAULT_FILE_SIZE   50*1024*1024

extern int __log_level__;
extern int __hex_level__;

#define log_generic(lvl, fmt, args...) write_log(lvl, __FILE__, __FUNCTION__, __LINE__ , fmt, ##args)
#define TRACE(fmt, args...)		    log_generic(-1, fmt, ##args)
#define log_emerg(fmt, args...)	    log_generic(0, fmt, ##args)
#define log_alert(fmt, args...)		log_generic(1, fmt, ##args)
#define log_crit(fmt, args...)		log_generic(2, fmt, ##args)
#define log_error(fmt, args...)		log_generic(3, fmt, ##args)
#define log_warning(fmt, args...)	do{ if(__log_level__>=4)log_generic(4, fmt, ##args); } while(0)
#define log_notice(fmt, args...)	do{ if(__log_level__>=5)log_generic(5, fmt, ##args); } while(0)
#define log_info(fmt, args...)		do{ if(__log_level__>=6)log_generic(6, fmt, ##args); } while(0)
#define log_debug(fmt, args...)		do{ if(__log_level__>=7)log_generic(7, fmt, ##args); } while(0)
#define hex_debug(head,size,str)	do{ if(__hex_level__>0)write_hex(__hex_level__, __FILE__, __FUNCTION__, __LINE__, head, size, str); } while(0)

extern void init_log(const char* app, const char* dir = NULL, int max_num = -1, int max_size = -1);
extern void set_log_level(int);
extern void toggle_hex_level();
extern void write_log(int, const char*, const char*, int, const char*, ...) __attribute__((format(__printf__,5,6)));
extern void write_hex(int level, const char* filename, const char* funcname, int lineno, const char* head, int size, char* str);

__END_DECLS

#endif
