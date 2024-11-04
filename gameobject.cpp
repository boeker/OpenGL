#include "gameobject.h"

GameObject::GameObject(Model *model, Shader *shader) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    velocity = 0.0f;
    falling = false;

    this->model = model;
    this->shader = shader;
}

glm::vec3 GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec3 &newPosition) {
    position = newPosition;
}

void GameObject::simulateGravity(float deltaTime) {
    if (falling) {
        float newVelocity = 0.0f; //velocity + GRAVITY * deltaTime;
        float displacement = deltaTime * (velocity + newVelocity) / 2.0f;
        position.y -= displacement;

        velocity = newVelocity;
    }
}

void GameObject::draw() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    //TODO
    //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    shader->setMat4("model", model);

    this->model->draw();
}
