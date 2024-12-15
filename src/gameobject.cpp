#include "gameobject.h"

#include "game.h"

void GameObject::updateFront() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
}

GameObject::GameObject(Model *model) {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    yaw = 0.0f;
    pitch = 0.0f;
    updateFront();
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    velocity = 0.0f;
    gravity = true;
    falling = true;

    this->model = model;
    heightOffset = 0.0f;
    yawOffset = 0.0f;

    shader = nullptr;
    borderShader = nullptr;
    drawBorder = false;
}

glm::vec3 GameObject::getPosition() const {
    return position;
}

glm::vec3 GameObject::getFront() const {
    return front;
}

glm::vec3 GameObject::getUp() const {
    return up;
}

float GameObject::getYaw() const {
    return yaw;
}

float GameObject::getPitch() const {
    return pitch;
}

void GameObject::setPosition(const glm::vec3 &position) {
    this->position = position;
}

void GameObject::setHeightOffset(const float offset) {
    heightOffset = offset;
}

void GameObject::setYawOffset(const float offset) {
    yawOffset = offset;
}

void GameObject::setGravity(bool gravity) {
    this->gravity = gravity;
}

void GameObject::processDirectionChange(float yawOffset, float pitchOffset) {
    yaw += yawOffset;
    pitch += pitchOffset;

    updateFront();
}

void GameObject::setDirection(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;

    updateFront();
}

void GameObject::move(glm::vec3 direction) {
    position += direction;
}

void GameObject::jump() {
    if (!falling) {
        falling = true;
        velocity = -8.0f;
    }
}

void GameObject::simulateGravity(float deltaTime, float mapHeight) {
    if (gravity) {
        if (position.y > mapHeight && !falling) {
            falling = true;
            velocity = 0.0f;
        }

        if (falling) {
            float newVelocity = velocity + GRAVITY * deltaTime;
            float displacement = deltaTime * (velocity + newVelocity) / 2.0f;
            position.y -= displacement;

            velocity = newVelocity;
        }

        if (position.y < mapHeight) {
            falling = false;
            velocity = 0.0f;
            position.y = mapHeight;
        }
    }
}


void GameObject::draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.0f, heightOffset, 0.0f));
    model = glm::rotate(model, glm::radians(yaw + yawOffset), glm::vec3(0.0f, -1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(pitch), glm::vec3(0.0f, 0.0f, 1.0f));
    shader.setMat4("model", model);

    this->model->draw(shader);
}

void GameObject::drawBorderObject(Shader &shader) {
    if (drawBorder) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::translate(model, glm::vec3(0.0f, heightOffset, 0.0f));
        model = glm::rotate(model, glm::radians(yaw + yawOffset), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(1.1f));
        shader.setMat4("model", model);

        this->model->draw(shader);
    }
}

void GameObject::setShader(Shader *shader) {
    this->shader = shader;
}

void GameObject::setBorderShader(Shader *shader) {
    this->borderShader = shader;
}

Shader* GameObject::getShader() {
    return shader;
}

Shader* GameObject::getBorderShader() {
    return borderShader;
}

void GameObject::setDrawBorder(bool drawBorder) {
    this->drawBorder = drawBorder;
}

void GameObject::draw() {
    if (shader) {
        shader->use();
        draw(*shader);
    }
}

void GameObject::drawBorderObject() {
    if (borderShader) {
        borderShader->use();
        drawBorderObject(*borderShader);
    }
}
