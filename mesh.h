#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <assimp/scene.h>

#include "shader.h"
#include "texture.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

extern std::vector<Texture> loadedTextures;

class Mesh {
private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    unsigned int VAO; // vertex attribute object
    unsigned int VBO; // vertex buffer object
    unsigned int EBO; // element buffer object
    
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures);

    void setUpMesh();
    void draw(Shader &shader);

    static Mesh fromAssimpMesh(aiMesh *mesh, const aiScene *scene, std::string &directory);
    static std::vector<Texture> loadMaterialTextures(aiMaterial *material,
                                                           aiTextureType type,
                                                           std::string typeName,
                                                           std::string &directory); 
    static Mesh cubeMesh();
};

#endif
