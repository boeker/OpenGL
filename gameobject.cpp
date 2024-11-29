#include "gameobject.h"

#include "game.h"

void GameObject::updateFront() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

GameObject::GameObject(Model *model, Game *game) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    yaw = 0.0f;
    pitch = 0.0f;
    updateFront();
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    velocity = 0.0f;
    falling = true;

    this->model = model;
    heightOffset = 0.0f;
    yawOffset = 0.0f;

    this->game = game;
}

glm::vec3 GameObject::getPosition() const {
    return position;
}

glm::vec3 GameObject::getFront() const {
    return front;
}

glm::vec3 GameObject::getUp() const {
    return up;
}

float GameObject::getYaw() const {
    return yaw;
}

float GameObject::getPitch() const {
    return pitch;
}

void GameObject::setPosition(const glm::vec3 &position) {
    this->position = position;
}

void GameObject::setHeightOffset(const float offset) {
    heightOffset = offset;
}

void GameObject::setYawOffset(const float offset) {
    yawOffset = offset;
}

void GameObject::processDirectionChange(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    pitch += pitchOffset;

    updateFront();
}

void GameObject::setDirection(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;

    updateFront();
}

void GameObject::move(glm::vec3 direction) {
    position += direction;
}

void GameObject::jump() {
    if (!falling) {
        falling = true;
        velocity = -8.0f;
    }
}

void GameObject::simulateGravity(float deltaTime) {
    float currentHeight = game->map->getHeight(position.x, position.z);

    if (position.y > currentHeight && !falling) {
        falling = true;
        velocity = 0.0f;
    }

    if (falling) {
        float newVelocity = velocity + GRAVITY * deltaTime;
        float displacement = deltaTime * (velocity + newVelocity) / 2.0f;
        position.y -= displacement;

        velocity = newVelocity;
    }

    if (position.y < currentHeight) {
        falling = false;
        velocity = 0.0f;
        position.y = currentHeight;
    }

}


void GameObject::draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.0f, heightOffset, 0.0f));
    model = glm::rotate(model, glm::radians(yaw + yawOffset), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(pitch), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setMat4("model", model);

    this->model->draw(shader);
}
