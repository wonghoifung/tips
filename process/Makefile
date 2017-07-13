src = $(wildcard *.cpp)
obj = $(patsubst %.cpp, %.o, $(src))

CC = g++
CPPFLAGS = -pipe -Wall -O2

ifeq ($(shell arch), i686)
	CPPFLAGS += -march=i686
endif

ifneq ($(shell grep -c ^processor /proc/cpuinfo 2>/dev/null), 1)
	CPPFLAGS += -DCONFIG_SMP
endif

TARGET = app

all:: $(TARGET)

${TARGET}:$(obj)
	${CC} -o $@ ${CPPFLAGS} $^ $(LIB)

%.o: %.c
	${CC} ${CPPFLAGS} -c -o $@ $<

clean:
	@rm -rf $(obj) $(TARGET)
