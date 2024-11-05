#ifndef MODEL_H
#define MODEL_H

#include "shader.h"
#include "texture.h"

class Model {
private:
    unsigned int VAO; // vertex attribute object
    unsigned int VBO; // vertex buffer object

    float *vertices;
    unsigned int numOfVertices;
    Texture texture;

    Shader *shader;

public:
    Model(Texture &texture, Shader *shader);
    
    void setGeometry(float *vertices, unsigned int size);
    void transferGeometry();
    void draw();
    
    Shader* getShader();
};

#endif
