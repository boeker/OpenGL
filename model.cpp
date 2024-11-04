#include "model.h"

#include "glad/glad.h"
#include "glm/glm.hpp"

Model::Model(Texture &texture) {
    this->texture = texture;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Model::setGeometry(float *vertices, unsigned int num) {
    this->vertices = vertices;
    this->numOfVertices = num;
}

void Model::transferGeometry() {
    // bind
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, numOfVertices * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::draw() {
    glActiveTexture(GL_TEXTURE0);
    texture.bind();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, numOfVertices);
}

