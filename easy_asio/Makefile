TARGET = gate

PREFIX_BIN =

CXX=g++  

ENV_INC = /Users/wonghoifung/myenvironment/include
ENV_LIB = /Users/wonghoifung/myenvironment/lib

ifeq (0,${debug}) 
	CPPFLAGS = -DNDEBUG -g2 -O2 
else
	CPPFLAGS = -ggdb -O2 
endif 

CPPFLAGS += -Wall
CPPFLAGS += -I$(ENV_INC) 

LIBS = -lboost_date_time -lboost_system  
LINKFLAGS = -L/usr/local/lib 
LINKFLAGS += -L$(ENV_LIB)

INCLUDEDIRS = ./  
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./  

SOURCEDIRS = $(MYSOURCEDIRS)


C_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.c))
C_OBJS = $(patsubst %.c, %.o, $(C_SOURCES))


CPP_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.cpp))
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))

all:compile
.PHONY :all

.c.o:
	$(CC) -c -o $*.o $(CFLAGS) $(INCLUDES) $*.c
.cpp.o:
	$(CXX) -c -o $*.o $(CPPFLAGS) $(INCLUDES) $*.cpp

compile: $(CPP_OBJS) $(C_OBJS) $(OTHERS_C_OBJS) $(OTHERS_CPP_OBJS)
	$(CXX) $(LINKFLAGS) -o $(TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(CPP_OBJS) $(C_OBJS)
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(PREFIX_BIN)

uninstall:
	rm -f $(PREFIX)/$(PREFIX_BIN)

rebuild: clean

