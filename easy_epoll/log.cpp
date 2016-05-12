#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "log.h"

#define LOGSIZE         1 * 1024 * 1024
#define MAX_PATH_LEN    256

int __stdin__ = 0; // TODO
int __log_level__ = 6;
int __bef_log_level__ = 0;
int __hex_level__ = 0;

static char log_dir[128] = ".";
static char appname[32];
static int  max_file_num    = DEFAULT_FILE_NUM;
static int  max_file_size   = DEFAULT_FILE_SIZE;
static int  cur_file_pos    = 0;
static char logTT[] = {'0','1','2','E','W','N','I','D','D'};

void init_log(const char* app, const char* dir, int max_num, int max_size)
{
    strncpy(appname, app, sizeof(appname)-1);
    if(dir) strncpy(log_dir, dir, sizeof(log_dir) - 1);
    mkdir(log_dir, 0777);
    if(access(log_dir, W_OK|X_OK) < 0) log_error("logdir(%s): not writable", log_dir);
    if(max_num > 0) max_file_num = max_num;
    if(max_size > 0) max_file_size = max_size;
}

void write_to_stdin(int i) 
{
    __stdin__ = i;
}

void set_log_level(int l)
{
    if(l>=0) __log_level__ = l > 4 ? l : 4;
}

void reset_hex_level()
{
    __hex_level__^=1;
    if(__hex_level__==0) {
    	__log_level__ = __bef_log_level__;
    } else {
    	__bef_log_level__ = __log_level__;
    	__log_level__ = 7;
    }
    log_notice("log_level=[%d],hex_level=[%d],bef_log_level=[%d]",__log_level__,__hex_level__,__bef_log_level__);
}

void write_access(int access, const char* rsp_buf, const char* fmt, ...)
{
    if (0 == access) return;

    char  rspinfo[MAX_PATH_LEN] = {'\0'};
    int   rsplen                = 0;
    char* checkpoint            = strstr ((char *)rsp_buf, "\r\n");

    if (NULL == checkpoint) return;

    rsplen = checkpoint - rsp_buf;
    if(rsplen > MAX_PATH_LEN - 1) rsplen = MAX_PATH_LEN - 1;

    memcpy (rspinfo, rsp_buf, rsplen);
    rspinfo[rsplen] = 0x00;

    // save errno
    int savedErrNo = errno;
    int off = 0;
    char buf[LOGSIZE];
    char logfile[MAX_PATH_LEN];

    struct tm tm;
    time_t now = time(NULL);
    localtime_r(&now, &tm);
    off = snprintf(buf, LOGSIZE - 1, "[%02d:%02d:%02d] : response info[%s] ", tm.tm_hour, tm.tm_min, tm.tm_sec, rspinfo);

    snprintf(logfile, MAX_PATH_LEN - 1, "%s/%s.access%04d%02d%02d.log",
            log_dir, appname,
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    va_list ap;
    va_start(ap, fmt);
    // restore errno
    errno = savedErrNo;
    off += vsnprintf(buf+off, LOGSIZE-off-2, fmt, ap);
    va_end(ap);

    if(buf[off-1] != '\n') buf[off++] = '\n';

    if(appname[0]) {
        int fd = open (logfile, O_CREAT | O_LARGEFILE | O_APPEND |O_WRONLY, 0644);

        if (fd >= 0) {
            write(fd, buf, off);
            close (fd);
        }
    }
}

static int access_log()
{
	static int fd = -1;
	static time_t lasttime = 0;
	static int linenum = 0;
	static int lastline = 0;
	bool btruncate = false;
	int times = 10;
	do {
		time_t now = time(0);
		++linenum;
		if( now-lasttime < 10 && linenum-lastline < 1000 && fd > 0 ) {
			return fd;
		} else if( fd > 0 ) {
			close(fd);
			fd = -1;
		}
		char logfile[512];
		struct stat stBuf;

		if(!appname[0]) {
			fprintf(stderr, "ATTENTION: the prefix of the log file is not set\n");
			return -1;
		}
		snprintf(logfile, MAX_PATH_LEN-1, "%s/%s_%d.log",log_dir, appname,cur_file_pos);
		int flags = O_CREAT | O_LARGEFILE | O_APPEND | O_WRONLY;
		if(btruncate) {
			flags |= O_TRUNC;
		}
		fd = open(logfile, flags, 0644);
		if (fd < 0) {
			fprintf(stderr, "ATTENTION: open log file FAILED, dir[%s] file[%s], error[%m]\n", log_dir, appname);
			break;
		}
		if(-1 == fstat(fd, &stBuf)) {
			fprintf(stderr, "ATTENTION: stat log file FAILED, dir[%s] file[%s], error[%m]\n", log_dir, appname);
			close(fd);
			fd = -1;
			break;
		}

		if((int)stBuf.st_size >= max_file_size) {
			cur_file_pos = (cur_file_pos + 1)%max_file_num;
			close(fd);
			fd = -1;
			btruncate = true;
			continue;
		}
		lasttime = now;
		break;
	} while (--times);
	return fd;
}

void write_log(int level, const char* filename, const char* funcname, int lineno, const char* format, ...)
{
    int savedErrNo = errno;
    int off = 0;
    int n_write;
    char buf[LOGSIZE];
    int fd = -1;
    struct tm tm;
    time_t now = time (NULL);

    localtime_r(&now, &tm);
    filename = basename(filename);
    off = snprintf(buf, LOGSIZE-1,
            "[%c]%02d%02d-%02d:%02d:%02d|%s(%d)%s|",
            logTT[level],tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec,
            filename, lineno, funcname);
    if((fd = access_log())<=0) return;

    va_list ap;
    va_start(ap, format);
    // restore errno
    errno = savedErrNo;
    n_write= vsnprintf(buf+off, LOGSIZE-off-2, format, ap);
    va_end(ap);

    if(n_write>=LOGSIZE-off-2)
        off=LOGSIZE-2;
    else
        off+=n_write;
    
    if(buf[off-1] != '\n')
        buf[off++] = '\n';

    if (-1 == level)
        fwrite(buf, 1, off, stderr);

    write(fd, buf, off);
}

static unsigned char* formatstr(unsigned char* s,int size)
{
	int i=0;
	for(;i<size;i++) {
		if(s[i]=='\0' || s[i]=='\n') {
			s[i] = '.';
		} else if (s[i]<33) {
			s[i] = '*';
		}
	}
	return s;
}

void write_hex(int level, const char* filename, const char* funcname, int lineno, const char* head, int size, char* str)
{
    if(level<=0) return;
    int fd = -1;
    struct tm tm;
    struct timeval tv;
    time_t now = time (NULL);
    gettimeofday(&tv,NULL);

    localtime_r(&now, &tm);
    filename = basename(filename);

    if((fd = access_log())<=0) return;

    int i,j,end;
    int line=0;
    char buffer[100];
	unsigned char buf[17];
	memset(buffer,0,100);
	sprintf(buffer,"%02d%02d-%02d:%02d:%02d.%06ld|%s(%d)%s|%s,size=[%d]\n",
	        tm.tm_mon + 1, tm.tm_mday,tm.tm_hour, tm.tm_min, tm.tm_sec,tv.tv_usec,
	        filename, lineno, funcname,head,size);
	write(fd, buffer,strlen(buffer));
    memset(buf,0,17);
	memset(buffer,0,100);
	line = size/16;
	for(i=0;i<line;i++) {
		memcpy(buf,str+16*i,16);
		sprintf(buffer,"%04X | %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X | %s\n",(i+1)*16,(unsigned char)buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],buf[12],buf[13],buf[14],buf[15],formatstr(buf,16));
		write(fd, buffer, strlen(buffer));
	}
	end = size - 16*i;
	if(end > 0) {
		memset(buf,0,17);
		memset(buffer,0,100);
		memcpy(buf,str+16*i,end);
		sprintf(buffer,"%04X |",i*16+end);
		for(j=0;j<end;j++) {
			sprintf(buffer+6+j*3," %02X",buf[j]);
		}
		for(;j<16;j++) {
			sprintf(buffer+6+j*3,"   ");
		}
		sprintf(buffer+54," | %s\n",formatstr(buf,end));
		write(fd, buffer, strlen(buffer));
	}
}

