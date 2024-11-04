#ifndef MODEL_H
#define MODEL_H

#include "texture.h"

class Model {
private:
    unsigned int VAO; // vertex attribute object
    unsigned int VBO; // vertex buffer object

    float *vertices;
    unsigned int numOfVertices;
    Texture texture;

public:
    Model(Texture &texture);
    
    void setGeometry(float *vertices, unsigned int size);
    void transferGeometry();
    void draw();
    
};

#endif
