#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "glm/glm.hpp"

#include "shader.h"
#include "model.h"

class Game;

class GameObject {
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;

    float velocity;
    bool falling;

    Game *game;

    Model *model;
    float heightOffset;
public:
    enum class Direction {FORWARD, BACKWARD, LEFT, RIGHT};

    GameObject(Model *model, Game *game);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3 &newPosition);
    void setHeightOffset(const float &offset);

    void processMovement(Direction direction, float deltaTime);
    void jump();

    void draw();
    void simulateGravity(float deltaTime);
};

#endif
