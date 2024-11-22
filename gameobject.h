#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glm/glm.hpp"

#include "shader.h"
#include "model.h"
#include "constants.h"

class Game;

class GameObject {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float yaw;
    float pitch;

    float velocity;
    bool falling;

    Game *game;

    Model *model;
    float heightOffset;
    float yawOffset;

public:
    GameObject(Model *model, Game *game);

    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getUp() const;
    void setPosition(const glm::vec3 &newPosition);
    void setHeightOffset(const float offset);
    void setYawOffset(const float offset);

    void processDirectionChange(float yawOffset, float pitchOffset);
    void updateFront();
    void setDirection(float yaw, float pitch);
    float getYaw();
    float getPitch();
    
    void move(glm::vec3 direction);
    void jump();

    void draw(Shader &shader);
    void simulateGravity(float deltaTime);
};

#endif
