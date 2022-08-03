target: common.o dfa.o lexical.o main.o
	g++ -Wall -std=c++14 common.o dfa.o lexical.o main.o -o main

common.o: common.hpp common.cpp
	g++ -Wall -std=c++14 -c common.cpp

dfa.o: dfa.hpp dfa.cpp
	g++ -Wall -std=c++14 -c dfa.cpp

lexical.o: lexical.hpp lexical.cpp
	g++ -Wall -std=c++14 -c lexical.cpp

main.o: main.hpp main.cpp
	g++ -Wall -std=c++14 -c main.cpp

run: target
	./main fonte.alg > saida.txt

clean:
	rm *.o
