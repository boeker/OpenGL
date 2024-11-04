#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glm/glm.hpp"

#include "shader.h"
#include "model.h"

class GameObject {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float velocity;
    bool falling;

    Model *model;
    Shader *shader;
public:
    GameObject(Model *model, Shader *shader);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &newPosition);

    void draw();
    void simulateGravity(float deltaTime);
};

#endif
