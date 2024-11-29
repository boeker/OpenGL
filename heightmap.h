#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <iostream>

#include "mesh.h"

#include "glm/glm.hpp"

class HeightMap {
private:
    float **heightMap;
    glm::vec3 ***normals;

    glm::vec3 computeNormal(unsigned int x, unsigned int y);

public:
    static const int SIZE = 200;

    HeightMap();
    ~HeightMap();

    void generateMap();
    Mesh generateMesh();
    float getHeight(float x, float y) const;
};

#endif

