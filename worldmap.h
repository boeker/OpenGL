#ifndef WORLDMAP_H 
#define WORLDMAP_H

#include <iostream>

#include "mesh.h"

#include "glm/glm.hpp"

class WorldMap {
private:
    glm::vec3 computeNormal(unsigned int x, unsigned int y);

public:
    static const int SIZE = 200;

    float **heightMap;
    float *vertexList;

    glm::vec3 ***normals;

    WorldMap();
    ~WorldMap();

    void generateMap();
    Mesh generateMesh();
    int generateVertexList();
    float getHeight(float x, float y);
};

#endif
