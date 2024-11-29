#ifndef CAMERA_H 
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "gameobject.h"
#include "game.h"
#include "constants.h"

class Camera {
private:
    // position of camera
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);

    // pitch and yaw of camera
    float pitch;
    float yaw;
    // front vector (determined by pitch and yaw)
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);

    // up vector
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // field of view
    float fov;

    // movement speed (camera and player)
    float movementSpeed;

    // mouse sensitivity
    float sensitivity;

    // is the player/camera spriting (moving faster)?
    bool sprinting;

    // follow a player object
    GameObject *player;
    // following player or fly mode?
    bool following; 
    // 1st person camera?
    bool firstPerson;
    // distance to player object in 3rd person
    float distance;

public:
    Camera();

    // compute view matrix
    glm::mat4 getViewMatrix() const;

    // compute new position, front, and up vector from pitch, yaw, and following player object
    void update();
    // move camera (and also following player object)
    void processMovement(Direction direction, float deltaTime);
    // change yaw and pitch (by offset, if permissible, also of player object)
    void processDirectionChange(float yawOffset, float pitchOffset);
    // change FOV (by offset, if permissible)
    void processFOVChange(float offset);

    // getters and setters
    float getFOV() const;
    glm::vec3 getPosition() const;
    void setSprinting(bool running);
    glm::vec3 getPlayerPOVPosition() const;
    glm::vec3 getPlayerPOVFront() const;

    // follow a player object
    void attachToPlayer(GameObject *player);
    void detachFromPlayer();
    bool isFollowing();
    void setFollowing(bool following);
    void adjustDistance(float offset);
};

#endif
