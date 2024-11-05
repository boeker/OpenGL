#include "gameobject.h"

#include "game.h"

GameObject::GameObject(Model *model, Game *game) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    yaw = 0.0f;
    pitch = 0.0f;

    velocity = 0.0f;
    falling = true;

    this->model = model;
    heightOffset = 0.0f;
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

void GameObject::setPosition(const glm::vec3 &newPosition) {
    position = newPosition;
}

void GameObject::setHeightOffset(const float &offset) {
    heightOffset = offset;
}

void GameObject::simulateGravity(float deltaTime) {
    float currentHeight = game->worldMap->getHeight(position.x, position.z);

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

void GameObject::move(glm::vec3 direction) {
    position += direction;
}

void GameObject::processMovement(Direction direction, float deltaTime) {
    float previousHeight = position.y;
    const float cameraSpeed = 5.0f * deltaTime;//movementSpeed * deltaTime;

    if (direction == Direction::FORWARD) {
        position += cameraSpeed * front;
    }

    if (direction == Direction::BACKWARD) {
        position -= cameraSpeed * front;
    }

    if (direction == Direction::LEFT) {
        position -= cameraSpeed * glm::normalize(glm::cross(front, up));
    }

    if (direction == Direction::RIGHT) {
        position += cameraSpeed * glm::normalize(glm::cross(front, up));
    }

    //position.y = previousHeight;
}

void GameObject::processDirectionChange(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    pitch += pitchOffset;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

void GameObject::jump() {
    if (!falling) {
        falling = true;
        velocity = -15.0f;
    }
}

void GameObject::draw() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.0f, heightOffset, 0.0f));
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(pitch), glm::vec3(0.0f, 0.0f, 1.0f));
    this->model->getShader()->setMat4("model", model);

    this->model->draw();
}
