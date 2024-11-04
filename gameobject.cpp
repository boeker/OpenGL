#include "gameobject.h"

#include "game.h"

GameObject::GameObject(Model *model, Shader *shader, Game *game) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    velocity = 0.0f;
    falling = true;

    this->model = model;
    this->shader = shader;
    this->game = game;
}

glm::vec3 GameObject::getPosition() const {
    return position;
}

void GameObject::setPosition(const glm::vec3 &newPosition) {
    position = newPosition;
}

void GameObject::simulateGravity(float deltaTime) {
    float currentHeight = game->worldMap->getHeight(position.x, position.z);
    std::cout << "current height" << currentHeight << std::endl;

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

    std::cout << "gravity " << position.y << std::endl;
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
    std::cout << "x" << position.x << std::endl;
    std::cout << "y" << position.y << std::endl;
    std::cout << "z" << position.z << std::endl;
}

void GameObject::draw() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    //TODO
    //model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.3f, 0.5f));
    shader->setMat4("model", model);

    this->model->draw();
}
