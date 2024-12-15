#include "game.h"

Game::Game(Camera &camera)
    : lightsourceShader("shaders/light.vs", "shaders/lightsource.fs"),
      lightShader("shaders/materialLighting.vs", "shaders/materialLighting.fs"),
      lightingShader("shaders/lighting.vs", "shaders/lighting.fs"),
      borderShader("shaders/materialLighting.vs", "shaders/border.fs") {

    // generate height map and create model from it
    heightMap.generateMap();
    Model* mapModel = new Model(heightMap.generateMesh());

    mapObject = new GameObject(mapModel);
    mapObject->setShader(&lightingShader);
    mapObject->setGravity(false);

    // Load backpack model and use it as player object
    Model *backpack = new Model;
    backpack->loadModel("models/backpack/backpack.obj");

    Texture wall = Texture::createTextureFromFile("textures/wall.jpg", "texture_diffuse");

    Model *crateModel = new Model(Mesh::cubeMesh());

    // player
    GameObject *playerObject = new GameObject(backpack);
    playerObject->setShader(&lightingShader);
    playerObject->setBorderShader(&borderShader);
    playerObject->setDrawBorder(false);
    playerObject->setHeightOffset(1.7f);
    playerObject->setYawOffset(90.0f);
    playerObject->setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    playerObject->setDirection(45.0f, 0.0f);
    camera.attachToPlayer(playerObject);
    addGameObject(playerObject);

    // material cube on top of mountain
    GameObject *materialCube = new GameObject(crateModel);
    materialCube->setShader(&lightShader);
    materialCube->setBorderShader(&borderShader);
    materialCube->setPosition(glm::vec3(99.0f, 50.5f, 99.0f));
    materialCube->setHeightOffset(0.5f);
    materialCube->setDrawBorder(true);
    addGameObject(materialCube);

    // crate on top of mountain
    GameObject *crate = new GameObject(crateModel);
    crate->setShader(&lightingShader);
    crate->setBorderShader(&borderShader);
    crate->setPosition(glm::vec3(101.0f, 42.0f, 101.0f));
    crate->setHeightOffset(0.5f);
    crate->setDrawBorder(true);
    addGameObject(crate);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-2.8f,  3.0f,  7.5f)  
    };
    for (unsigned int i = 0; i < 10; ++i) {
        GameObject *rotatingCrate = new GameObject(crateModel);
        rotatingCrate->setShader(&lightingShader);
        rotatingCrate->setBorderShader(&borderShader);
        rotatingCrate->setPosition(glm::vec3(102.0f, 41.0f, 102.0f) + cubePositions[i]);
        rotatingCrate->setHeightOffset(0.5f);
        rotatingCrate->setGravity(false);
        rotatingCrate->setDrawBorder(true);

        float angle = 0.0f;
        rotatingCrate->setDirection(angle, angle);

        addGameObject(rotatingCrate);
        rotatingCrates.push_back(rotatingCrate);
    }

    // light source object
    lightSourceObject = new GameObject(crateModel);
    lightSourceObject->setShader(&lightsourceShader);
    lightSourceObject->setBorderShader(&borderShader);
    lightSourceObject->setHeightOffset(0.5f);
    lightSourceObject->setDrawBorder(true);
    lightSourceObject->setGravity(false);
    lightSourceObject->setScale(0.2f);
    addGameObject(lightSourceObject);
}

void Game::addGameObject(GameObject *object) {
    gameObjects.push_back(object);
}

void Game::setMapObject(GameObject *object) {
    mapObject = object;
}

void Game::simulateGravity(float deltaTime) {
    for (GameObject *object : gameObjects) {
        glm::vec3 position = object->getPosition();

        object->simulateGravity(deltaTime, heightMap.getHeight(position.x, position.z));
    }
}

void Game::draw(Shader &shader) {
    for (GameObject *object : gameObjects) {
        object->draw(shader);
    }
}

void Game::draw() {
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // Replace if both depth and stencil tests pass
    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Stencil test always passes
    glStencilMask(0x00); // Disable writing to stencil buffer
    if (mapObject) {
        mapObject->draw();
    }

    glStencilMask(0xFF); // Enable writing to stencil buffer

    for (GameObject *object : gameObjects) {
        object->draw();
    }

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Stencil test passes outside of drawn objects
    glStencilMask(0x00); // Disable writing to stencil buffer
    glDisable(GL_DEPTH_TEST); // Make sure that floor (for example) does not overwrite borders

    for (GameObject *object : gameObjects) {
        object->drawBorderObject();
    }

    glStencilFunc(GL_ALWAYS, 1, 0xFF); // Stencil test always passes
    glEnable(GL_DEPTH_TEST); // Re-enable depth test
}

void Game::processGameLogic(float time, const glm::vec3 &lightPosition) {
    // rotate crates
    for (unsigned int i = 0; i < 10; ++i) {
        float angle = 20.0f * i + 50.0f * time;
        rotatingCrates[i]->setDirection(angle, angle);
    }

    // position light
    lightSourceObject->setPosition(lightPosition);
}
