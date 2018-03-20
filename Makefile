all:make

make: shell.cpp
	g++ -std=c++14 -lboost_system -lboost_filesystem shell.cpp -o myshell
