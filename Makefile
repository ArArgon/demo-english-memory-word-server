.PHONY: build
build:
	g++ src/*/*.cpp src/main.cpp -I/usr/local/include -I/home/cpp/include -lhiredis -lmysqlclient -o test
