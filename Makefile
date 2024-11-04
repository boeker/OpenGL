CXXFLAGS = -g -Wall -lglfw
CXX = g++

CXXSOURCES = main.cpp\
	camera.cpp\
	gameobject.cpp\
	game.cpp\
	player.cpp\
	shader.cpp\
	stb_image.cpp\
	texture.cpp\
	worldmap.cpp

CSOURCES = glad.c

HEADERS = camera.h\
	gameobject.h\
	game.h\
	player.h\
	shader.h\
	stb_image.h\
	texture.h\
	worldmap.h

OBJECTS = $(CXXSOURCES:.cpp=.o) $(CSOURCES:.c=.o)

#$(info	Objects files: $(OBJECTS))

all: main

main: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(CXXSOURCES:.cpp=.o): %.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CSOURCES:.c=.o): %.o: %.c $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	-@rm main $(OBJECTS) 2>/dev/null

