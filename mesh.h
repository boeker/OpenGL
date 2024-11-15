#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <assimp/scene.h>

#include "shader.h"

struct VertexStruct {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 textureCoordinates;
};

struct TextureStruct {
    unsigned int id;
    std::string type;
    std::string pathOfFile;
};

extern std::vector<TextureStruct> loadedTextures;

class Mesh {
private:
    std::vector<VertexStruct> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureStruct> textures;

    unsigned int VAO; // vertex attribute object
    unsigned int VBO; // vertex buffer object
    unsigned int EBO; // element buffer object
    
public:
    Mesh(std::vector<VertexStruct> vertices,
         std::vector<unsigned int> indices,
         std::vector<TextureStruct> textures);

    void setUpMesh();
    void draw(Shader &shader);

    static Mesh fromAssimpMesh(aiMesh *mesh, const aiScene *scene, std::string &directory);
    static std::vector<TextureStruct> loadMaterialTextures(aiMaterial *material,
                                                           aiTextureType type,
                                                           std::string typeName,
                                                           std::string &directory); 
    static unsigned int createTextureFromFile(const char *path, const std::string &directory);
};

#endif
