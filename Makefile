.PHONY: build
build:
	g++ src/*/*.cpp src/main.cpp -I/usr/local/include -I./include -lhiredis -lmysqlclient -std=c++11 -O2 -Wall -Wunused -o server
