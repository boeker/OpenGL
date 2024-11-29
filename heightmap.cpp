#include "heightmap.h"

#include "glm/gtx/string_cast.hpp"

HeightMap::HeightMap() {
    heightMap = new float*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        heightMap[i] = new float[SIZE];
    }

    normals = new glm::vec3**[SIZE - 1];

    for (int i = 0; i < SIZE - 1; ++i) {
        normals[i] = new glm::vec3*[SIZE - 1];
        
        for (int j = 0; j < SIZE - 1; ++j) {
            normals[i][j] = new glm::vec3[2];
        }
    }
}

HeightMap::~HeightMap() {
    for (int i = 0; i < SIZE; ++i) {
        delete[] heightMap[i];
    }
    delete[] heightMap;

    for (int i = 0; i < SIZE - 1; ++i) {
        
        for (int j = 0; j < SIZE - 1; ++j) {
            delete[] normals[i][j];
        }
        delete[] normals[i];
    }
    delete[] normals;
}

void HeightMap::generateMap() {
    heightMap[0][0] = 0.0f;

    for (int x = 1; x < SIZE; ++x) {
        int heightOffset = (rand() % 100) - 50;
        float heightOffsetFloat = (float)heightOffset / 400.0f;

        heightMap[x][0] = heightMap[x - 1][0] + heightOffsetFloat;
    }

    for (int y = 1; y < SIZE; ++y) {
        int heightOffset = (rand() % 100) - 50;
        float heightOffsetFloat = (float)heightOffset / 400.0f;

        heightMap[0][y] = heightMap[0][y - 1] + heightOffsetFloat;
    }

    for (int y = 1; y < SIZE; ++y) {
        for (int x = 1; x < SIZE; ++x) {
            float prevX = heightMap[x - 1][y];
            float prevY = heightMap[x][y - 1];
            float average = (prevX + prevY) / 2.0f;

            int heightOffset = (rand() % 100) - 50;
            float heightOffsetFloat = (float)heightOffset / 100.0f;
            if (y < SIZE / 2 && x < SIZE / 2) {
                heightOffsetFloat += 0.2f;
            } else {
                heightOffsetFloat -= 0.2f;
            }

            heightMap[x][y] = average + heightOffsetFloat;
        }
    }
}

glm::vec3 HeightMap::computeNormal(unsigned int x, unsigned int y) {
    std::vector<glm::vec3> normalCandidates;

    if (x > 0 && y < SIZE - 1) {
        normalCandidates.push_back(normals[x - 1][y][0]);
        normalCandidates.push_back(normals[x - 1][y][1]);
    }

    if (y > 0 && x < SIZE - 1) {
        normalCandidates.push_back(normals[x][y - 1][0]);
        normalCandidates.push_back(normals[x][y - 1][1]);
    }

    if (x > 0 && y > 0) {
        normalCandidates.push_back(normals[x - 1][y - 1][1]);
    }

    if (x < SIZE - 1 && y < SIZE - 1) {
        normalCandidates.push_back(normals[x][y][0]);
    }

    glm::vec3 normal(0.0f, 0.0f, 0.0f);
    for (const glm::vec3 &vec : normalCandidates) {
        normal += vec;
    }

    return (1.0f / (float)normalCandidates.size()) * normal;
}

Mesh HeightMap::generateMesh() {
    std::vector<Texture> textures;
    Texture texture = Texture::createTextureFromFile("textures/moon2.jpg", "texture_diffuse");
    textures.push_back(texture);
    Texture texture_specular = Texture::createTextureFromFile("textures/moon2_specular.jpg", "texture_specular");
    textures.push_back(texture_specular);

    // generate normals
    for (int x = 1; x < SIZE; ++x) {
        for (int y = 1; y < SIZE; ++y) {
            // current square
            glm::vec3 ll = glm::vec3((float) (x - 1), heightMap[x - 1][y - 1], (float) (y - 1));
            glm::vec3 lr = glm::vec3((float) x, heightMap[x][y - 1], (float) (y - 1));
            glm::vec3 ul = glm::vec3((float) (x - 1), heightMap[x - 1][y], (float) y);
            glm::vec3 ur = glm::vec3((float) x, heightMap[x][y], (float) y);

            glm::vec3 firstNormal = glm::normalize(glm::cross(ul - ll, lr - ll));
            glm::vec3 secondNormal = glm::normalize(glm::cross(ur - ul, lr - ul));

            normals[x - 1][y - 1][0] = firstNormal;
            normals[x - 1][y - 1][1] = secondNormal;

        }
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int x = 1; x < SIZE; ++x) {
        for (int y = 1; y < SIZE; ++y) {
            // current square
            glm::vec3 ll = glm::vec3((float) (x - 1), heightMap[x - 1][y - 1], (float) (y - 1));
            glm::vec3 llNormal = computeNormal(x - 1, y - 1);

            glm::vec3 lr = glm::vec3((float) x, heightMap[x][y - 1], (float) (y - 1));
            glm::vec3 lrNormal = computeNormal(x, y - 1);

            glm::vec3 ul = glm::vec3((float) (x - 1), heightMap[x - 1][y], (float) y);
            glm::vec3 ulNormal = computeNormal(x - 1, y);

            glm::vec3 ur = glm::vec3((float) x, heightMap[x][y], (float) y);
            glm::vec3 urNormal = computeNormal(x, y);

            // generate two triangles for the current square
            Vertex vertex;

            // first triangle
            vertex.position = ll;
            vertex.normal = llNormal;
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = ul;
            vertex.normal = ulNormal;
            vertex.textureCoordinates = glm::vec2(0.0f, 1.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = lr;
            vertex.normal = lrNormal;
            vertex.textureCoordinates = glm::vec2(1.0f, 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            // second triangle
            vertex.position = ul;
            vertex.normal = ulNormal;
            vertex.textureCoordinates = glm::vec2(0.0f, 1.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = lr;
            vertex.normal = lrNormal;
            vertex.textureCoordinates = glm::vec2(1.0f, 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = ur;
            vertex.normal = urNormal;
            vertex.textureCoordinates = glm::vec2(1.0f, 1.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

        }
    }

    return Mesh(vertices, indices, textures);
}

float HeightMap::getHeight(float x, float y) {
    if (x < 0 || y < 0 || x > SIZE || y > SIZE) {
        return -std::numeric_limits<float>::infinity();
    }

    int xInt = (int)floor(x);
    int yInt = (int)floor(y);

    float ll = heightMap[xInt][yInt]; 
    float lr = heightMap[xInt + 1][yInt]; 
    float ul = heightMap[xInt][yInt + 1]; 
    float ur = heightMap[xInt + 1][yInt + 1]; 

    float s = x - (float)xInt;
    float t = y - (float)yInt;

    return (1 - t) * ((1 - s) * ll + s * lr) + t * ((1 - s) * ul + s * ur);
} 

