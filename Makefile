all:
	mkdir data/
	g++ main.cpp -o server -lpthread
	./server
