#include "camera.h"

Camera::Camera() {
    attached = false;

    firstPerson = false;
    distance = 5.0f;

    position = glm::vec3(3.0f, 3.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    pitch = 0.0f;
    yaw = 90.0f;

    fov = 45.0f;

    sensitivity = 0.1f;
    movementSpeed = 5.0f;

    running = false;
}

void Camera::attachToPlayer(GameObject *player) {
    this->player = player;
    this->attached = true;
    //firstPerson = true;

    yaw = player->getYaw();
    pitch = player->getPitch();
}

void Camera::detachFromPlayer() {
    this->player = nullptr;
    this->attached = false;
}

bool Camera::isAttached() {
    return attached;
}

void Camera::attach() {
    attached = true;
}

void Camera::detach() {
    attached = false;
}

void Camera::update() {
    // compute new front vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    if (player && attached) {
        if (firstPerson) {
            // first person camera
            up = player->getUp();
            position = player->getPosition() + 1.5f * front + 3.0f * up;

        } else {
            // third person camera
            up = player->getUp();
            position = player->getPosition() - distance * front + 2.0f * up;
            front = glm::normalize(player->getPosition() - position);
        }
    }
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processMovement(Direction direction, float deltaTime) {
    const float cameraSpeed = ((running) ? (movementSpeed * 5.0f)
                                         : movementSpeed)* deltaTime;
    glm::vec3 directionVector;

    switch (direction) {
        case Direction::FORWARD:
            directionVector = front;
            break;
        case Direction::BACKWARD:
            directionVector = -front;
            break;
        case Direction::LEFT:
            directionVector = -glm::normalize(glm::cross(front, up));
            break;
        case Direction::RIGHT:
            directionVector = glm::normalize(glm::cross(front, up));
            break;
        case Direction::UPWARD:
            directionVector = up;
            break;
        case Direction::DOWNWARD:
            directionVector = -up;
            break;
    }

    position += cameraSpeed * directionVector;

    if (player && attached) {
        if (direction == Direction::UPWARD) {
            player->jump();
        } else {
            glm::vec3 playerMovement = directionVector;
            playerMovement.y = 0.0f;
            if (playerMovement.x != 0.0f || playerMovement.z != 0.0f) {
                player->move(cameraSpeed * glm::normalize(playerMovement));
                player->setDirection(yaw, 0.0f);
            }
        }
    }
}

void Camera::processDirectionChange(float yawOffset, float pitchOffset) {
    yaw += yawOffset * sensitivity;
    pitch += pitchOffset * sensitivity;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    if (player && attached && firstPerson) {
        player->processDirectionChange(yawOffset * sensitivity, 0.0f);
    }
}

void Camera::processFOVChange(float offset) {
    fov -= offset;
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    if (fov > 45.0f) {
        fov = 45.0f;
    }
}

void Camera::adjustDistance(float offset) {
    distance += offset;
    if (distance < 1.0f) {
        distance = 1.0f;
    }

    if (!firstPerson && distance < 2.0f) {
        firstPerson = true;
        distance = 1.0f;
        player->setDirection(yaw, 0.0f);
    }

    if (firstPerson && distance >= 2.0f) {
        firstPerson = false;
    }
}

void Camera::setRunning(bool running) {
    this->running = running;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getPlayerPOVPosition() const {
    if (this->player != nullptr && !firstPerson) {
        return player->getPosition() + 1.5f * front + 3.0f * up;

    } else {
        return position;
    }
}

glm::vec3 Camera::getPlayerPOVFront() const {
    if (this->player != nullptr && !firstPerson) {
        return player->getFront();

    } else {
        return front;
    }
}
