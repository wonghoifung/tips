TARGET = libee.so

BUILD_DIR =

CXX=g++  

ifeq (0,${debug}) 
	CPPFLAGS = -DNDEBUG -g2 -O2 
else
	CPPFLAGS = -ggdb -O2 
endif 

CPPFLAGS += -Wall
CPPFLAGS += -I/usr/local/include

LIBS = -lrt
LINKFLAGS = -L/usr/local/lib 

INCLUDEDIRS = ./ 
INCLUDES = $(foreach tmp, $(INCLUDEDIRS), -I $(tmp))

MYSOURCEDIRS = ./ 

SOURCEDIRS = $(MYSOURCEDIRS)

CPP_SOURCES = $(foreach tmp, $(SOURCEDIRS), $(wildcard $(tmp)*.cpp))
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SOURCES))

all:compile
.PHONY : all

.cpp.o:
	$(CXX) -fPIC -c -o $*.o $(CPPFLAGS) $(INCLUDES) $*.cpp

compile: $(CPP_OBJS) 
	$(CXX) $(LINKFLAGS) -shared -o $(TARGET) $^ $(LIBS)

.PHONY : clean
clean:
	rm -f $(CPP_OBJS) 
	rm -f $(TARGET)

install: $(TARGET)
	cp $(TARGET) $(BUILD_DIR)

uninstall:
	rm -f $(PREFIX)/$(BUILD_DIR)

rebuild: clean
