TARGET = luatest

PREFIX_BIN =

CC=gcc

ifeq (0,${debug}) 
	CFLAGS = -DNDEBUG -g2 -O2 -std=gnu99 
else
	CFLAGS = -ggdb -O2 -std=gnu99 -DDEBUG_LOG
endif 

CFLAGS += -Wall
CFLAGS += -I/home/albert/lua-5.3.2/src/

LIBS = -lrt -ldl -lm -llua
#LINKFLAGS = -L/usr/local/lib 
LINKFLAGS += -L/home/albert/lua-5.3.2/src/ 

INCLUDEDIRS = ./ 
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./ 

SOURCEDIRS = $(MYSOURCEDIRS)

C_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.c))
C_OBJS = $(patsubst %.c, %.o, $(C_SOURCES))

LUA_CLIB_PATH ?= testlib

LUA_CLIB = counter luahello profile

all:compile \
	$(foreach v, $(LUA_CLIB), $(LUA_CLIB_PATH)/$(v).so) 
.PHONY :all

SHARED := -fPIC --shared

$(LUA_CLIB_PATH)/counter.so : testlib/counter.c | $(LUA_CLIB_PATH)
	$(CC) $(CFLAGS) $(LINKFLAGS) $(LIBS) $(SHARED) $^ -o $@

$(LUA_CLIB_PATH)/luahello.so : testlib/luahello.c | $(LUA_CLIB_PATH)
	$(CC) $(CFLAGS) $(LINKFLAGS) $(LIBS) $(SHARED) $^ -o $@

$(LUA_CLIB_PATH)/profile.so : testlib/profile.c | $(LUA_CLIB_PATH)
	$(CC) $(CFLAGS) $(LINKFLAGS) $(LIBS) $(SHARED) $^ -o $@

.c.o:
	$(CC) -c -o $*.o $(CFLAGS) $(INCLUDES) $*.c

compile:  $(C_OBJS) $(OTHERS_C_OBJS)
	$(CC) $(LINKFLAGS) -o $(TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(C_OBJS)
	rm -f $(TARGET)
	rm -f ./testlib/*.o ./testlib/*.so

install: $(TARGET)
	cp $(TARGET) $(PREFIX_BIN)

uninstall:
	rm -f $(PREFIX)/$(PREFIX_BIN)

rebuild: clean

