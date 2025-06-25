all:put get

put:test_put.cpp
	g++ -o $@ $^ -std=c++17 -letcd-cpp-api -lcpprest -lspdlog -lfmt
get:test_get.cpp
	g++ -o $@ $^ -std=c++17 -letcd-cpp-api -lcpprest -lspdlog -lfmt

.PHONY:clean
clean:
	rm -f put get