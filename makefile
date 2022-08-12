target: common.o dfa.o lexical.o syntatic.o main.o
	g++ -Wall -std=c++14 common.o dfa.o lexical.o syntatic.o main.o -o main

common.o: common.hpp common.cpp
	g++ -Wall -std=c++14 -c common.cpp

dfa.o: lexical/dfa.hpp lexical/dfa.cpp
	g++ -Wall -std=c++14 -c lexical/dfa.cpp

lexical.o: lexical/lexical.hpp lexical/lexical.cpp
	g++ -Wall -std=c++14 -c lexical/lexical.cpp

syntatic.o: syntatic/syntatic.hpp syntatic/syntatic.cpp
	g++ -Wall -std=c++14 -c syntatic/syntatic.cpp

main.o: main.hpp main.cpp
	g++ -Wall -std=c++14 -c main.cpp

run: target
	./main fonte.alg > saida.txt

clean:
	rm *.o
