#include "game.h"

Game::Game(WorldMap *worldMap) {
    this->worldMap = worldMap;
}

void Game::draw() {
    for (GameObject &object : gameObjects) {
        object.draw();
    }
}
