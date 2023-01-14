CC=g++  
CXXFLAGS = -std=c++0x
CFLAGS=-I
skiplist: main.o 
	$(CC) -o ./main main.o --std=c++11 -pthread 
	rm -f ./*.o

test: skipListTest.o
	$(CC) -o ./skipListTest skipListTest.o --std=c++11 -pthread 
	rm -f ./*.o

client: client.o
	$(CC) -o ./client client.o --std=c++11 -pthread 
	rm -f ./*.o

clean: 
	rm -f ./*.o
