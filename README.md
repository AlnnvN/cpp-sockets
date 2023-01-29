# cpp-sockets
Base implementation of a client and a server using native C++ UNIX socket libraries

TCP
g++ -o build/tcpclient tcpclient.cpp

./tcpclient [host] [port]

UDP
g++ -o build/udpclient udpclient.cpp

./udpclient [host] [port]
