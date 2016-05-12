BUILD_PATH ?= .
CXX=g++  
CPPFLAGS = -g -O2 -Wall 

HIREDIS_INC ?= /home/albert/hiredis/include/hiredis
HIREDIS_LIB ?= /home/albert/hiredis/lib

UNITTEST_REDIS_SRC = redisClient.cpp redisManager.cpp unittest_redisClient.cpp
UNITTEST_REDIS_OBJ = redisClient.o redisManager.o unittest_redisClient.o

all : redis

redis : $(BUILD_PATH)/unittest_redisClient

redisClient.o : redisClient.cpp
	$(CXX) -c -o redisClient.o $(CPPFLAGS) -I$(HIREDIS_INC) redisClient.cpp
redisManager.o : redisManager.cpp
	$(CXX) -c -o redisManager.o $(CPPFLAGS) -I$(HIREDIS_INC) redisManager.cpp
unittest_redisClient.o : unittest_redisClient.cpp
	$(CXX) -c -o unittest_redisClient.o $(CPPFLAGS) -I$(HIREDIS_INC) unittest_redisClient.cpp

$(BUILD_PATH)/unittest_redisClient : $(UNITTEST_REDIS_OBJ)
	$(CXX) -L$(HIREDIS_LIB) -lhiredis -lrt -o $(BUILD_PATH)/unittest_redisClient $^

clean: 
	rm -f *.o $(BUILD_PATH)/unittest_redisClient