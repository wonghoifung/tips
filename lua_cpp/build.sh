g++ -I../ -L../ -fPIC --shared cppcurl.cpp -o cppcurl.so
# gcc -I../ -fPIC --shared cppcurl.c -o cppcurl.so
g++ -I../ -L../ main.cpp cppcurl.so -o test ../liblua.a -ldl
