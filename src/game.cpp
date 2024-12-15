#include "game.h"

Game::Game(Shader *shader) {
    // generate height map and create model from it
    heightMap.generateMap();
    Model* mapModel = new Model(heightMap.generateMesh());

    mapObject = new GameObject(mapModel);
    mapObject->setShader(shader);
    mapObject->setGravity(false);
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
