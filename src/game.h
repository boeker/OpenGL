#ifndef GAME_H
#define GAME_H

#include <vector>

#include "camera.h"

#include "gameobject.h"
#include "heightmap.h"
#include "shader.h"

class Game {
private:
    std::vector<GameObject*> gameObjects;
    HeightMap heightMap;
    GameObject *mapObject;

public:
    Camera camera;
    
    // Game-specific objects
    Shader lightsourceShader;
    Shader lightShader;
    Shader lightingShader;
    Shader borderShader;
    std::vector<GameObject*> rotatingCrates;
    GameObject *lightSourceObject;

    glm::vec3 whiteLight;
    glm::vec3 redLight;
    glm::vec3 lightPosition;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;


    glm::vec3 lightDirection;
    glm::mat3 normalMatrix;
    glm::vec3 vsLightPosition;
    glm::vec3 vsLightDirection;
    glm::vec3 vsPlayerPosition;
    glm::vec3 vsPlayerFront;
    bool flashlight;

public:
    Game();

    void addGameObject(GameObject *object);
    void setMapObject(GameObject *object);
    void simulateGravity(float deltaTime);
    void draw(Shader &shader);
    void draw();
    void processGameLogic(float time);
    void setUpShaders();
    void setUpLightingShader(Shader &shader);
};

#endif
