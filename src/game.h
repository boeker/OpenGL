#ifndef GAME_H
#define GAME_H

#include <vector>

#include "camera.h"
#include "gameobject.h"
#include "heightmap.h"
#include "shader.h"

class Camera;

class Game {
private:
    std::vector<GameObject*> gameObjects;
    HeightMap heightMap;
    GameObject *mapObject;
    
public:
    // Game-specific objects
    Shader lightsourceShader;
    Shader lightShader;
    Shader lightingShader;
    Shader borderShader;
    std::vector<GameObject*> rotatingCrates;
    GameObject *lightSourceObject;

public:
    Game(Camera &camera);

    void addGameObject(GameObject *object);
    void setMapObject(GameObject *object);
    void simulateGravity(float deltaTime);
    void draw(Shader &shader);
    void draw();
    void processGameLogic(float time, const glm::vec3 &lightPosition);
};

#endif
