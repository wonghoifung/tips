g++ -fPIC -c dy.cpp
g++ -shared -o libdy.so dy.o

g++ -fPIC -c dy2.cpp
g++ -shared -o libdy2.so dy2.o

g++ -c st.cpp
ar -cr libst.a st.o
