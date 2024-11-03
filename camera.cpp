#include "camera.h"

Camera::Camera() {
    position = glm::vec3(0.0f, 3.0f, 3.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    pitch = 0.0f;
    yaw = -90.0f;

    fov = 45.0f;

    movementSpeed = 5.0f;
    sensitivity = 0.1f;

    velocity = 0.0f;
    falling = true;
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

    position.y = previousHeight;
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

void Camera::processGravity(float deltaTime) {
    if (falling) {
        float newVelocity = velocity + GRAVITY * deltaTime;
        float displacement = deltaTime * (velocity + newVelocity) / 2.0f;
        position.y -= displacement;

        velocity = newVelocity;
    }
}
