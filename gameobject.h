#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glm/glm.hpp"

#include "shader.h"

class GameObject {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float velocity;
    bool falling;

    unsigned int VAO;
    float *geometry;
    unsigned int geometryLength;
    unsigned int texture;

public:
    GameObject();

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &newPosition);

    void simulateGravity(float deltaTime);
    void draw();
};

#endif
