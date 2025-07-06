all:discover register

discover:discover.cpp
	g++ -o $@ $^ -std=c++17 -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest -lbrpc -lssl -lcrypto -lprotobuf -lleveldb

register:register.cpp
	g++ -o $@ $^ -std=c++17 -lspdlog -lfmt -lgflags -letcd-cpp-api -lcpprest -lbrpc -lssl -lcrypto -lprotobuf -lleveldb
 