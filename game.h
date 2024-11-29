#ifndef GAME_H
#define GAME_H

#include <vector>

#include "gameobject.h"
#include "heightmap.h"

class Game {
private:
    std::vector<GameObject> gameObjects;

public:
    HeightMap *map;

    Game(HeightMap *map);
};

#endif
