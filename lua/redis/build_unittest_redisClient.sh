
g++ -I/home/albert/hiredis/include/hiredis/ -L/home/albert/hiredis/lib/ -lhiredis -lrt redisClient.cpp redisManager.cpp unittest_redisClient.cpp -o unittest_redisClient