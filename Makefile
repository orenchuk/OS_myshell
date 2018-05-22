all:make

make: shell.cpp
	g++ -std=c++14 -lboost_system -lboost_filesystem -lboost_system shell.cpp -o myshell

.PHONY: clean

clean: 
	rm myshell
