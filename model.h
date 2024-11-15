#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include <assimp/scene.h>

class Model {
private:
    // old
    unsigned int VAO; // vertex attribute object
    unsigned int VBO; // vertex buffer object

    float *vertices;
    unsigned int numOfVertices;
    Texture texture;

    Shader *shader;

    // new
    std::string directory;
    std::vector<Mesh> meshes;

    void processScene(const aiScene *scene, std::string &directory);

public:
    // old
    Model(Texture &texture, Shader *shader);
    
    void setGeometry(float *vertices, unsigned int size);
    void transferGeometry();
    void draw();
    
    Shader* getShader();

    // new
    void loadModel(const std::string &path);
    void draw(Shader &shader);
};

#endif
