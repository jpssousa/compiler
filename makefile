target: common.o dfa.o lexical.o syntactic.o main.o
	g++ -Wall -std=c++14 common.o dfa.o lexical.o syntactic.o main.o -o main

common.o: common.hpp common.cpp
	g++ -Wall -std=c++14 -c common.cpp

dfa.o: lexical/dfa.hpp lexical/dfa.cpp
	g++ -Wall -std=c++14 -c lexical/dfa.cpp

lexical.o: lexical/lexical.hpp lexical/lexical.cpp
	g++ -Wall -std=c++14 -c lexical/lexical.cpp

syntactic.o: syntactic/syntactic.hpp syntactic/syntactic.cpp
	g++ -Wall -std=c++14 -c syntactic/syntactic.cpp

main.o: main.hpp main.cpp
	g++ -Wall -std=c++14 -c main.cpp

run: target
	./main fonte.alg > saida.txt

clean:
	rm *.o
