FLAGS = -g -Wall -lglfw

main: main.o glad.o
	g++ $(FLAGS) main.o glad.o -o main

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

glad.o: glad.c
	g++ $(FLAGS) -c glad.c

clean:
	rm main main.o glad.o

