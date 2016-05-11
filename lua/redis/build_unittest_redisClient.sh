git clone https://github.com/redis/hiredis.git
cd hiredis
make
cd ..
g++ -I./hiredis/ -L./hiredis/ -lhiredis redisClient.cpp unittest_redisClient.cpp -o unittest_redisClient