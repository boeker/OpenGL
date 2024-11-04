#ifndef GAME_H
#define GAME_H

#include <vector>

#include "gameobject.h"
#include "worldmap.h"

class Game {
private:
    std::vector<GameObject> gameObjects;

public:
    WorldMap *worldMap;

    Game(WorldMap *worldMap);

    static constexpr float GRAVITY = 9.82f;

    void draw();
};

#endif
