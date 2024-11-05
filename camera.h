#ifndef CAMERA_H 
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "gameobject.h"

class Camera {
private:
    GameObject *player;
    bool attached;
    bool firstPerson;

public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float pitch;
    float yaw;

    float fov;

    float movementSpeed;

    float velocity;

    enum class Direction {FORWARD, BACKWARD, LEFT, RIGHT, UPWARD, DOWNWARD};
    float sensitivity;


    Camera();
    void attachToPlayer(GameObject *player);
    void detachFromPlayer();
    bool isAttached();
    void attach();
    void detach();

    void update();
    glm::mat4 getViewMatrix() const;
    
    void processMovement(Direction direction, float deltaTime);
    void processDirectionChange(float yawOffset, float pitchOffset);
    void processFOVChange(float offset);
};

#endif
