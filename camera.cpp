#include "camera.h"

Camera::Camera() {
    position = glm::vec3(3.0f, 3.0f, 3.0f);
    pitch = 0.0f;
    yaw = 90.0f;
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    fov = 45.0f;
    movementSpeed = 5.0f;
    sensitivity = 0.1f;
    sprinting = false;

    following = false;
    firstPerson = false;
    distance = 5.0f;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::update() {
    // compute new front vector
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);

    if (player && following) {
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

void Camera::processMovement(Direction direction, float deltaTime) {
    const float cameraSpeed = ((sprinting) ? (movementSpeed * 5.0f)
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

    if (player && following) {
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

    if (player && following && firstPerson) {
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

float Camera::getFOV() const {
    return fov;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

void Camera::setSprinting(bool sprinting) {
    this->sprinting = sprinting;
}

glm::vec3 Camera::getPlayerPOVPosition() const {
    if (this->player != nullptr && (!following || !firstPerson)) {
        return player->getPosition() + 1.5f * front + 3.0f * up;

    } else {
        return position;
    }
}

glm::vec3 Camera::getPlayerPOVFront() const {
    if (this->player != nullptr && (!following || !firstPerson)) {
        return player->getFront();

    } else {
        return front;
    }
}

void Camera::attachToPlayer(GameObject *player) {
    this->player = player;
    this->following = true;
    //firstPerson = true;

    yaw = player->getYaw();
    pitch = player->getPitch();
}

void Camera::detachFromPlayer() {
    this->player = nullptr;
    this->following = false;
}

bool Camera::isFollowing() {
    return following;
}

void Camera::setFollowing(bool following) {
    this->following = following;
}

void Camera::adjustDistance(float offset) {
    if (following) {
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
}

