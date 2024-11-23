#include "worldmap.h"

WorldMap::WorldMap() {
    heightMap = new float*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        heightMap[i] = new float[SIZE];
    }

    vertexList = nullptr;
}

WorldMap::~WorldMap() {
    for (int i = 0; i < SIZE; ++i) {
        delete[] heightMap[i];
    }
    delete[] heightMap;
}

void WorldMap::generateMap() {
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

Mesh WorldMap::generateMesh() {
    std::vector<Texture> textures;
    Texture texture = Texture::createTextureFromFile("textures/moon2.jpg", "texture_diffuse");
    textures.push_back(texture);

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int x = 1; x < SIZE; ++x) {
        for (int y = 1; y < SIZE; ++y) {
            // current square
            glm::vec3 ll = glm::vec3((float) (x - 1), heightMap[x - 1][y - 1], (float) (y - 1));
            glm::vec3 lr = glm::vec3((float) x, heightMap[x][y - 1], (float) (y - 1));
            glm::vec3 ul = glm::vec3((float) (x - 1), heightMap[x - 1][y], (float) y);
            glm::vec3 ur = glm::vec3((float) x, heightMap[x][y], (float) y);

            // generate two triangles for the current square
            Vertex vertex;

            // first triangle
            vertex.position = ll;
            vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.textureCoordinates = glm::vec2(0.0f, 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = ul;
            vertex.textureCoordinates = glm::vec2(0.0f, 1.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = lr;
            vertex.textureCoordinates = glm::vec2(1.0f, 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            // second triangle
            vertex.position = ul;
            vertex.textureCoordinates = glm::vec2(0.0f, 1.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = lr;
            vertex.textureCoordinates = glm::vec2(1.0f, 0.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);

            vertex.position = ur;
            vertex.textureCoordinates = glm::vec2(1.0f, 1.0f);
            indices.push_back(vertices.size());
            vertices.push_back(vertex);
        }
    }

    return Mesh(vertices, indices, textures);
}

int WorldMap::generateVertexList() {
    if (vertexList != nullptr) {
        delete vertexList;
    }
    int arraySize = 2 * 3 * 5 * (SIZE - 1) * (SIZE - 1);
    vertexList = new float[arraySize];

    for (int x = 1; x < SIZE; ++x) {
        for (int y = 1; y < SIZE; ++y) {
            // current square
            glm::vec3 ll = glm::vec3((float) (x - 1), heightMap[x - 1][y - 1], (float) (y - 1));
            glm::vec3 lr = glm::vec3((float) x, heightMap[x][y - 1], (float) (y - 1));
            glm::vec3 ul = glm::vec3((float) (x - 1), heightMap[x - 1][y], (float) y);
            glm::vec3 ur = glm::vec3((float) x, heightMap[x][y], (float) y);

            // generate two triangles for the current square
            int index = (x - 1) * 2 * 3 * 5 * (SIZE - 1) + 2 * 3 * 5 * (y - 1);
            vertexList[index + (0 * 5) + 0] = ll.x;
            vertexList[index + (0 * 5) + 1] = ll.y;
            vertexList[index + (0 * 5) + 2] = ll.z;
            vertexList[index + (0 * 5) + 3] = 0.0f;
            vertexList[index + (0 * 5) + 4] = 0.0f;

            vertexList[index + (1 * 5) + 0] = ul.x;
            vertexList[index + (1 * 5) + 1] = ul.y;
            vertexList[index + (1 * 5) + 2] = ul.z;
            vertexList[index + (1 * 5) + 3] = 0.0f;
            vertexList[index + (1 * 5) + 4] = 1.0f;

            vertexList[index + (2 * 5) + 0] = lr.x;
            vertexList[index + (2 * 5) + 1] = lr.y;
            vertexList[index + (2 * 5) + 2] = lr.z;
            vertexList[index + (2 * 5) + 3] = 1.0f;
            vertexList[index + (2 * 5) + 4] = 0.0f;

            vertexList[index + (3 * 5) + 0] = ul.x;
            vertexList[index + (3 * 5) + 1] = ul.y;
            vertexList[index + (3 * 5) + 2] = ul.z;
            vertexList[index + (3 * 5) + 3] = 0.0f;
            vertexList[index + (3 * 5) + 4] = 1.0f;

            vertexList[index + (4 * 5) + 0] = lr.x;
            vertexList[index + (4 * 5) + 1] = lr.y;
            vertexList[index + (4 * 5) + 2] = lr.z;
            vertexList[index + (4 * 5) + 3] = 1.0f;
            vertexList[index + (4 * 5) + 4] = 0.0f;

            vertexList[index + (5 * 5) + 0] = ur.x;
            vertexList[index + (5 * 5) + 1] = ur.y;
            vertexList[index + (5 * 5) + 2] = ur.z;
            vertexList[index + (5 * 5) + 3] = 1.0f;
            vertexList[index + (5 * 5) + 4] = 1.0f;
        }
    }

    return arraySize;
}

float WorldMap::getHeight(float x, float y) {
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
