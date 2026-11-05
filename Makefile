CC = g++

all: main

main: main 
	$(CC) -o main main.cpp

.PHONY: clean

clean:
	rm -f main