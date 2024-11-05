#include "camera.h"

Camera::Camera() {
    attached = false;

    firstPerson = false;

    position = glm::vec3(3.0f, 3.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    pitch = 0.0f;
    yaw = -90.0f;

    fov = 45.0f;

    sensitivity = 0.1f;
}

void Camera::attachToPlayer(GameObject *player) {
    this->player = player;
    this->attached = true;
    firstPerson = true;
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
    if (player && attached) {
        front = player->getFront();
        up = player->getUp();
        position = player->getPosition() + 0.5f * front;
    } 
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processMovement(Direction direction, float deltaTime) {
    float previousHeight = position.y;
    const float cameraSpeed = movementSpeed * deltaTime;

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
    if (direction == Direction::UPWARD) {
        position += cameraSpeed * up;
    }

    if (direction == Direction::DOWNWARD) {
        position -= cameraSpeed * up;
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

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
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

