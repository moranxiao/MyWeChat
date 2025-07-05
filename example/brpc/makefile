all:client server

client: client.cpp main.pb.cc
	g++ -o $@ $^  -std=c++17 -lbrpc -lgflags -lssl -lcrypto -lprotobuf -lleveldb

server: server.cpp main.pb.cc
	g++ -o $@ $^  -std=c++17 -lbrpc -lgflags -lssl -lcrypto -lprotobuf -lleveldb

.PHONY:clean
clean:
	rm -f client server