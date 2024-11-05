#include "gameobject.h"

#include "game.h"

GameObject::GameObject(Model *model, Game *game) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    velocity = 0.0f;
    falling = true;

    this->model = model;
    heightOffset = 0.0f;
    this->game = game;
}

glm::vec3 GameObject::getPosition() const {
    return position;
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
        float newVelocity = velocity + Game::GRAVITY * deltaTime;
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

void GameObject::jump() {
    if (!falling) {
        falling = true;
        velocity = -5.0f;
    }
}

void GameObject::draw() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.0f, heightOffset, 0.0f));
    //TODO
    //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    this->model->getShader()->setMat4("model", model);

    this->model->draw();
}
