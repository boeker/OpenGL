#include "game.h"

Game::Game(HeightMap *map) {
    this->map = map;
}

void Game::addGameObject(GameObject *object) {
    gameObjects.push_back(object);
}

void Game::simulateGravity(float deltaTime) {
    for (GameObject *object : gameObjects) {
        glm::vec3 position = object->getPosition();

        object->simulateGravity(deltaTime, map->getHeight(position.x, position.z));
    }
}

void Game::draw(Shader &shader) {
    for (GameObject *object : gameObjects) {
        object->draw(shader);
    }
}
