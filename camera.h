#ifndef CAMERA_H 
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float pitch;
    float yaw;

    float fov;

    float movementSpeed;
    float sensitivity;

    enum class Direction {FORWARD, BACKWARD, LEFT, RIGHT};

    Camera();

    glm::mat4 getViewMatrix() const;
    
    void processMovement(Direction direction, float deltaTime);
    void processDirectionChange(float yawOffset, float pitchOffset);
    void processFOVChange(float offset);
};

#endif
