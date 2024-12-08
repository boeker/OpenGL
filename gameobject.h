#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glm/glm.hpp>

#include "shader.h"
#include "model.h"
#include "constants.h"

class Game;

class GameObject {
private:
    // position of the object
    glm::vec3 position;
    // yaw and pitch, used to compute front vector
    float yaw;
    float pitch;
    glm::vec3 front;
    // up vector
    glm::vec3 up;

    // current movement velocity
    float velocity;

    // is this object affected by gravity?
    bool gravity;
    // is the object in mid-air?
    bool falling;

    // associated model
    Model *model;
    // position model relative to the following offsets
    float heightOffset;
    float yawOffset;

    // compute new front vector from yaw and pitch
    void updateFront();

public:
    GameObject(Model *model);

    // getters and setters
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getUp() const;
    float getYaw() const;
    float getPitch() const;
    void setPosition(const glm::vec3 &position);
    void setHeightOffset(const float offset);
    void setYawOffset(const float offset);
    void setGravity(bool gravity);

    // process (change of) yaw and pitch and update front vector
    void processDirectionChange(float yawOffset, float pitchOffset);
    void setDirection(float yaw, float pitch);

    // move object by direction
    void move(glm::vec3 direction);

    // launch object into air
    void jump();

    // compute new position after deltaTime
    void simulateGravity(float deltaTime, float mapHeight);

    // draw model of this object
    void draw(Shader &shader);
};

#endif
