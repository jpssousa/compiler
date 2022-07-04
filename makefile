CXX = g++
CXXFLAGS = -Wall -std=c++14

target: main.cpp lexical/lexical.cpp
	g++ -Wall -std=c++14 main.cpp lexical/lexical.cpp -o main

run: target
	./main fonte.alg
