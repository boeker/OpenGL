#ifndef GAME_H
#define GAME_H

#include <vector>

#include "gameobject.h"

class Game {
private:
    std::vector<GameObject> gameObjects;

public:
    static constexpr float GRAVITY = 9.82f;

    void draw();
};

#endif
