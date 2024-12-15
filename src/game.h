#ifndef GAME_H
#define GAME_H

#include <vector>

#include "gameobject.h"
#include "heightmap.h"
#include "shader.h"

class Game {
private:
    std::vector<GameObject*> gameObjects;
    HeightMap heightMap;
    GameObject *mapObject;

public:
    Game(Shader *shader);

    void addGameObject(GameObject *object);
    void setMapObject(GameObject *object);
    void simulateGravity(float deltaTime);
    void draw(Shader &shader);
    void draw();
};

#endif
