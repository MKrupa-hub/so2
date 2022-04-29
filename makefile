all: Etap1

Etap1: Etap1.cpp Board.hpp
	g++ Etap1.cpp -pthread -lncurses -o Etap1

clean:
	rm Etap1




