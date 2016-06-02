TARGET = client2

BUILD_DIR =

CXX=g++  

ifeq (0,${debug}) 
	CPPFLAGS = -DNDEBUG -g2 -O2 
else
	CPPFLAGS = -ggdb -O2 
endif 

CFLAGS += -Wall
CFLAGS += -I/usr/local/include
CFLAGS += -I../..

CPPFLAGS += -Wall
CPPFLAGS += -I/usr/local/include

LIBS = -lrt -lee2
LINKFLAGS = -L/usr/local/lib -L../..

INCLUDEDIRS = ./ 
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./ 

SOURCEDIRS = $(MYSOURCEDIRS)

C_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.c))
C_OBJS = $(patsubst %.c, %.o, $(C_SOURCES))

CPP_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.cpp))
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))

all:compile
.PHONY : all

.c.o:
	$(CC) -std=c99 -g -c -o $*.o $(CFLAGS) $(INCLUDES) $*.c

.cpp.o:
	$(CXX) -g -c -o $*.o $(CPPFLAGS) $(INCLUDES) $*.cpp

compile: $(CPP_OBJS) $(C_OBJS)
	$(CXX) $(LINKFLAGS) -o $(TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(CPP_OBJS) $(C_OBJS)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(BUILD_DIR)

uninstall:
	rm -f $(PREFIX)/$(BUILD_DIR)

rebuild: clean
