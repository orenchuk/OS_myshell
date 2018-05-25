all:make

make: shell.cpp
	g++ -std=c++14 shell.cpp -o myshell -lboost_system -lboost_filesystem -lboost_system 

.PHONY: clean

clean: 
	rm myshell
