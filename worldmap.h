#ifndef WORLDMAP_H 
#define WORLDMAP_H

#include <iostream>

#include "glm/glm.hpp"

class WorldMap {
public:
    static const int SIZE = 200;

    float **heightMap;
    float *vertexList;

    WorldMap();
    ~WorldMap();

    void generateMap();
    int generateVertexList();
    float getHeight(float x, float y);
};

#endif
