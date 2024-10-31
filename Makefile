FLAGS = -g -Wall -lglfw

main: main.o glad.o shader.o
	g++ $(FLAGS) main.o glad.o shader.o -o main

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

shader.o: shader.cpp
	g++ $(FLAGS) -c shader.cpp

glad.o: glad.c
	g++ $(FLAGS) -c glad.c

clean:
	rm main main.o shader.o glad.o

