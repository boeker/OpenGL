#include "game.h"

void Game::draw() {
    for (GameObject &object : gameObjects) {
        object.draw();
    }
}
