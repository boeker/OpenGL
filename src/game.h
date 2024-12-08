#ifndef GAME_H
#define GAME_H

#include <vector>

#include "gameobject.h"
#include "heightmap.h"
#include "shader.h"

class Game {
private:
    std::vector<GameObject*> gameObjects;

public:
    HeightMap *map;

    Game(HeightMap *map);

    void addGameObject(GameObject *object);
    void simulateGravity(float deltaTime);
    void draw(Shader &shader);
};

#endif
