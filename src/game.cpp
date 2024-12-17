#include "game.h"

#include "gl.h"

Game::Game(Camera *camera)
    : lightsourceShader("shaders/light.vs", "shaders/lightsource.fs"),
      lightShader("shaders/materialLighting.vs", "shaders/materialLighting.fs"),
      lightingShader("shaders/lighting.vs", "shaders/lighting.fs"),
      borderShader("shaders/materialLighting.vs", "shaders/border.fs"),
      whiteLight(1.0f, 1.0f, 1.0f),
      redLight(1.0f, 0.0f, 0.0f) {
    this->camera = camera;
    this->flashlight = false;

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
    this->camera->attachToPlayer(playerObject);
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

    // cube representing point light
    lightSourceObject = new GameObject(crateModel);
    lightSourceObject->setShader(&lightsourceShader);
    lightSourceObject->setBorderShader(&borderShader);
    lightSourceObject->setHeightOffset(0.5f);
    lightSourceObject->setDrawBorder(false);
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

void Game::processGameLogic(float time) {
    // rotate crates
    for (unsigned int i = 0; i < 10; ++i) {
        float angle = 20.0f * i + 50.0f * time;
        rotatingCrates[i]->setDirection(angle, angle);
    }

    //**********************************************************************
    // (view space) light positions
    //**********************************************************************

    // determine position of point light
    float angle = 20.0f * time;
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, glm::vec3(100.0f, 40.0f, 100.0f));
    matrix = glm::rotate(matrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

    lightPosition = matrix * glm::vec4(75.0f, 1.0f, 0.0f, 1.0f); 

    // position light
    lightSourceObject->setPosition(lightPosition);
}

void Game::setUpShaders() {
    //**********************************************************************
    // matrices
    //**********************************************************************

    // projection matrix
    projectionMatrix = glm::perspective(glm::radians(camera->getFOV()), 1280.0f / 720.0f, 0.1f, 300.0f);

    // view matrix
    camera->update();
    viewMatrix = camera->getViewMatrix();



    //**********************************************************************
    // view space light positions
    //**********************************************************************

    // direction of directional light
    lightDirection = glm::vec3(-2.0f, -1.0f, -0.0f);

    // determine positions in view space
    normalMatrix = (glm::transpose(glm::inverse(camera->getViewMatrix())));

    vsLightPosition = glm::vec3(viewMatrix * glm::vec4(lightPosition, 1.0f));
    vsLightDirection = glm::vec3(normalMatrix * lightDirection);
    vsPlayerPosition = glm::vec3(viewMatrix * glm::vec4(camera->getPlayerPOVPosition(), 1.0f));
    vsPlayerFront = glm::vec3(normalMatrix * camera->getPlayerPOVFront());

    //**********************************************************************
    // light source shader
    //**********************************************************************

    lightsourceShader.use();
    lightsourceShader.setVec3v("lightSourceColor", redLight);
    lightsourceShader.setMat4("view", viewMatrix);
    lightsourceShader.setMat4("projection", projectionMatrix);

    //**********************************************************************
    // lighting shader (material)
    //**********************************************************************

    lightShader.use();
    setUpLightingShader(lightShader);

    // set material properties
    lightShader.setVec3("material.ambient", 0.0f, 1.0f, 0.6f);
    lightShader.setVec3("material.diffuse", 0.0f, 1.0f, 1.0);
    lightShader.setVec3("material.specular", 0.5, 0.5, 0.5);
    lightShader.setFloat("material.shininess", 32.0f);

    // set transformations
    lightShader.setMat4("view", viewMatrix);
    lightShader.setMat4("projection", projectionMatrix);

    glCheckError();

    //**********************************************************************
    // lighting shader
    //**********************************************************************
    lightingShader.use();
    setUpLightingShader(lightingShader);

    // set material properties
    lightingShader.setVec3("material.specular", 0.5, 0.5, 0.5);
    lightingShader.setFloat("material.shininess", 32.0f);

    // set transformations
    lightingShader.setMat4("view", camera->getViewMatrix());
    lightingShader.setMat4("projection", projectionMatrix);

    borderShader.use();
    borderShader.setMat4("view", camera->getViewMatrix());
    borderShader.setMat4("projection", projectionMatrix);

    glCheckError();

    lightingShader.use();
    // draw game objects using lighting shader
    //game.draw(lightingShader, borderShader);
}

void Game::setUpLightingShader(Shader &shader) {
    // light source
    glm::vec3 ambientWhite = whiteLight * glm::vec3(0.1f); 
    glm::vec3 diffuseWhite = whiteLight * glm::vec3(0.8f); 
    glm::vec3 specularWhite = whiteLight * glm::vec3(1.0f); 

    glm::vec3 ambientRed = redLight * glm::vec3(0.1f); 
    glm::vec3 diffuseRed = redLight * glm::vec3(0.8f); 
    glm::vec3 specularRed = redLight * glm::vec3(1.0f); 

    float attenuationConstant = 1.0f;
    float attenuationLinear = 0.014f;
    float attenuationQuadratic = 0.0007f;

    glCheckError();

    // directional light
    shader.setVec3v("directionalLight.direction", vsLightDirection);
    shader.setVec3v("directionalLight.ambient", 0.1f * ambientWhite);
    shader.setVec3v("directionalLight.diffuse", 0.1f * diffuseWhite);
    shader.setVec3v("directionalLight.specular", 0.1f * specularWhite);

    glCheckError();

    // point light
    shader.setVec3v("pointLights[0].position", vsLightPosition);
    shader.setVec3v("pointLights[0].ambient", ambientRed);
    shader.setVec3v("pointLights[0].diffuse", diffuseRed);
    shader.setVec3v("pointLights[0].specular", specularRed);
    shader.setFloat("pointLights[0].constant", attenuationConstant);
    shader.setFloat("pointLights[0].linear", attenuationLinear);
    shader.setFloat("pointLights[0].quadratic", attenuationQuadratic);	

    glCheckError();

    // flashlight
    shader.setVec3v("spotLight.position", vsPlayerPosition);
    shader.setVec3v("spotLight.direction", vsPlayerFront);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    if (flashlight) {
        shader.setFloat("spotLight.enabled", 1.0f);
    } else {
        shader.setFloat("spotLight.enabled", 0.0f);
    }
    shader.setVec3v("spotLight.ambient", ambientWhite);
    shader.setVec3v("spotLight.diffuse", diffuseWhite);
    shader.setVec3v("spotLight.specular", specularWhite);
    shader.setFloat("spotLight.constant", attenuationConstant);
    shader.setFloat("spotLight.linear", attenuationLinear);
    shader.setFloat("spotLight.quadratic", attenuationQuadratic);	

    glCheckError();
}
