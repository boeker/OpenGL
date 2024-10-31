FLAGS = -g -Wall -lglfw

main: main.o glad.o shader.o stb_image.o
	g++ $(FLAGS) main.o glad.o shader.o stb_image.o -o main

main.o: main.cpp
	g++ $(FLAGS) -c main.cpp

shader.o: shader.cpp
	g++ $(FLAGS) -c shader.cpp

glad.o: glad.c
	g++ $(FLAGS) -c glad.c

stb_image.o: stb_image.cpp
	g++ $(FLAGS) -c stb_image.cpp

clean:
	rm main main.o shader.o glad.o stb_image.o

