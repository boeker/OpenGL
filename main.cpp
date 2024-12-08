#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"
#include "stb_image.h"

#include <iostream>
#include <cmath>

#include "gl.h"

#include "shader.h"
#include "camera.h"
#include "heightmap.h"
#include "game.h"
#include "gameobject.h"

#include "texture.h"
#include "model.h"
#include "mesh.h"

#include "constants.h"

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 640;
float lastY = 360;
bool firstMouse = true;

Camera camera;

bool flashlight = false;
bool keyCPressed = false;
bool keyFPressed = false;

glm::vec3 whiteLight(1.0f, 1.0f, 1.0f);
glm::vec3 redLight(1.0f, 0.0f, 0.0f);

void setUpLightingShader(Shader &shader,
                         const glm::vec3 &vsLightDirection,
                         const glm::vec3 &vsLightPosition,
                         const glm::vec3 &vsPlayerPosition,
                         const glm::vec3 &vsPlayerFront);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    std::cout << "framebuffer_size_callback " << width << "x" << height << std::endl;
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.processDirectionChange(xoffset,  yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.adjustDistance(-0.5f * (float)yoffset);
}

void processInput(GLFWwindow *window, GameObject *object) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processMovement(Direction::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processMovement(Direction::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processMovement(Direction::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processMovement(Direction::RIGHT, deltaTime);
    }

    camera.setSprinting(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.processMovement(Direction::UPWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processMovement(Direction::DOWNWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        //camera.velocity += -0.22f;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (!keyCPressed) {
            camera.setFollowing(!camera.isFollowing());
            keyCPressed = true;
        }
    } else {
        keyCPressed = false;
    }
    

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        if (!keyFPressed) {
            flashlight = !flashlight;
            keyFPressed = true;
        }
    } else {
        keyFPressed = false;
    }
}

int main(int argc, char *argv[]) {
    stbi_set_flip_vertically_on_load(true);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // the following is only needed on MacOS
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    glViewport(0, 0, 1280, 720);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

    Shader modelShader("shaders/model.vs", "shaders/model.fs");
    Shader lightMapsShader("shaders/lightmaps.vs", "shaders/lightmaps.fs");
    Shader lightsourceShader("shaders/light.vs", "shaders/lightsource.fs");
    Shader flashlightShader("shaders/flashlight.vs", "shaders/flashlight.fs");
    Shader lightShader("shaders/materialLighting.vs", "shaders/materialLighting.fs");
    Shader lightingShader("shaders/lighting.vs", "shaders/lighting.fs");

    // generate height map and create model from it
    HeightMap heightMap;
    heightMap.generateMap();
    Game game(&heightMap);

    Model mapModel(heightMap.generateMesh());
    GameObject mapObject(&mapModel);
    mapObject.setGravity(false);
    game.addGameObject(&mapObject);

    // Load backpack model and use it as player object
    modelShader.use();
    Model backpack;
    backpack.loadModel("models/backpack/backpack.obj");

    Texture wall = Texture::createTextureFromFile("textures/wall.jpg", "texture_diffuse");

    Model crateModel(Mesh::cubeMesh());

    // player
    GameObject playerObject(&backpack);
    playerObject.setHeightOffset(1.7f);
    playerObject.setYawOffset(90.0f);
    playerObject.setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    playerObject.setDirection(45.0f, 0.0f);
    camera.attachToPlayer(&playerObject);
    game.addGameObject(&playerObject);

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

    glEnable(GL_DEPTH_TEST);
    
    // wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input
        processInput(window, &playerObject);
        game.simulateGravity(deltaTime);

        glCheckError();

        // rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state-setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using function

        glCheckError();

        //**********************************************************************
        // matrices
        //**********************************************************************

        // projection matrix
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.getFOV()), 1280.0f / 720.0f, 0.1f, 300.0f);

        // view matrix
        camera.update();
        glm::mat4 viewMatrix = camera.getViewMatrix();

        //**********************************************************************
        // (view space) light positions
        //**********************************************************************

        // determine position of point light
        float angle = 20.0f * (float)glfwGetTime();
        glm::mat4 matrix = glm::mat4(1.0f);
        matrix = glm::translate(matrix, glm::vec3(100.0f, 40.0f, 100.0f));
        matrix = glm::rotate(matrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::vec3 lightPosition(matrix * glm::vec4(75.0f, 1.0f, 0.0f, 1.0f)); 

        // direction of directional light
        glm::vec3 lightDirection(-2.0f, -1.0f, -0.0f);

        // determine positions in view space
        glm::mat3 normalMatrix(glm::transpose(glm::inverse(camera.getViewMatrix())));

        glm::vec3 vsLightPosition = glm::vec3(viewMatrix * glm::vec4(lightPosition, 1.0f));
        glm::vec3 vsLightDirection(normalMatrix * lightDirection);
        glm::vec3 vsPlayerPosition(viewMatrix * glm::vec4(camera.getPlayerPOVPosition(), 1.0f));
        glm::vec3 vsPlayerFront(normalMatrix * camera.getPlayerPOVFront());

        //**********************************************************************
        // light source shader
        //**********************************************************************

        // draw cube representing point light
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, lightPosition);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    
        lightsourceShader.use();
        lightsourceShader.setVec3v("lightSourceColor", redLight);
        lightsourceShader.setMat4("view", viewMatrix);
        lightsourceShader.setMat4("projection", projectionMatrix);
        lightsourceShader.setMat4("model", modelMatrix);

        crateModel.draw(lightsourceShader);

        //**********************************************************************
        // lighting shader (material)
        //**********************************************************************

        lightShader.use();
        setUpLightingShader(lightShader, vsLightDirection, vsLightPosition, vsPlayerPosition, vsPlayerFront);

        // set material properties
        lightShader.setVec3("material.ambient", 0.0f, 1.0f, 0.6f);
        lightShader.setVec3("material.diffuse", 0.0f, 1.0f, 1.0);
    lightShader.setVec3("material.specular", 0.5, 0.5, 0.5);
    lightShader.setFloat("material.shininess", 32.0f);

        // set transformations
        lightShader.setMat4("view", viewMatrix);
        lightShader.setMat4("projection", projectionMatrix);
        // position cube on top of mountain
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(99.0f, 38.5f, 99.0f));
        lightShader.setMat4("model", modelMatrix);

        // draw using lighting shader (material)
        crateModel.draw(lightShader);

        glCheckError();

        //**********************************************************************
        // lighting shader
        //**********************************************************************
        lightingShader.use();
        setUpLightingShader(lightingShader, vsLightDirection, vsLightPosition, vsPlayerPosition, vsPlayerFront);

        // set material properties
        lightingShader.setVec3("material.specular", 0.5, 0.5, 0.5);
        lightingShader.setFloat("material.shininess", 32.0f);

        // set transformations
        lightingShader.setMat4("view", camera.getViewMatrix());
        lightingShader.setMat4("projection", projectionMatrix);
        // position crate on top of mountain
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(100.0f, 42.0f, 100.0f));
        lightingShader.setMat4("model", modelMatrix);

        // draw crate using lighting shader
        crateModel.draw(lightingShader);

        // draw rotating crates using lighting shader
        for (unsigned int i = 0; i < 10; ++i) {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(102.0f, 41.0f, 102.0f));
            float angle = 20.0f * i + 50.0f * (float)glfwGetTime();
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            lightingShader.setMat4("model", modelMatrix);

            crateModel.draw(lightingShader);

        }

        // draw game objects using lighting shader
        game.draw(lightingShader);


        glCheckError();

        // swap buffers
        glfwSwapBuffers(window);

        // check and call events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void setUpLightingShader(Shader &shader,
                         const glm::vec3 &vsLightDirection,
                         const glm::vec3 &vsLightPosition,
                         const glm::vec3 &vsPlayerPosition,
                         const glm::vec3 &vsPlayerFront) {
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
