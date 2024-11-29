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

Mesh generateCubeMesh() {
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, 
                                                             
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
                                                             
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                                             
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
                                                             
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
                                                             
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
    };

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Vertex *asVertices = (Vertex*)cubeVertices;
    for (int i = 0; i < 36; ++i) {
        vertices.push_back(asVertices[i]);
        indices.push_back(i);
    }


    std::vector<Texture> textures;
    Texture cont = Texture::createTextureFromFile("textures/container2.png", "texture_diffuse");
    Texture contSpecular = Texture::createTextureFromFile("textures/container2_specular.png", "texture_specular");
    textures.push_back(cont);
    textures.push_back(contSpecular);

    return Mesh(vertices, indices, textures);
}

float mixFactor = 0.5f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 640;
float lastY = 360;
bool firstMouse = true;

Camera camera;

bool flashlight = false;

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
        camera.setFollowing(!camera.isFollowing());
    }
    

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        flashlight = !flashlight;
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
    Shader lightShader("shaders/light.vs", "shaders/light.fs");
    Shader lightMapsShader("shaders/lightmaps.vs", "shaders/lightmaps.fs");
    Shader lightsourceShader("shaders/light.vs", "shaders/lightsource.fs");
    Shader flashlightShader("shaders/flashlight.vs", "shaders/flashlight.fs");
    Shader lightingShader("shaders/lighting.vs", "shaders/lighting.fs");

    // generate height map and create model from it
    HeightMap heightMap;
    heightMap.generateMap();
    Game game(&heightMap);

    Model mapModel(heightMap.generateMesh());
    GameObject mapObject(&mapModel, &game);

    // Load backpack model and use it as player object
    modelShader.use();
    Model backpack;
    backpack.loadModel("models/backpack/backpack.obj");

    Texture wall = Texture::createTextureFromFile("textures/wall.jpg", "texture_diffuse");

    Model crateModel(generateCubeMesh());

    // player
    GameObject playerObject(&backpack, &game);
    playerObject.setHeightOffset(1.7f);
    playerObject.setYawOffset(90.0f);
    playerObject.setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    playerObject.setDirection(45.0f, 0.0f);
    camera.attachToPlayer(&playerObject);

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
        playerObject.simulateGravity(deltaTime);

        // rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state-setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using function

        glCheckError();
        modelShader.use();

        camera.update();
        modelShader.setMat4("view", camera.getViewMatrix());

        glCheckError();

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.getFOV()), 1280.0f / 720.0f, 0.1f, 300.0f);
        modelShader.setMat4("projection", projection);

        glCheckError();
        
        //playerObject.draw(modelShader);

        // light source
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 redLight(1.0f, 0.0f, 0.0f);
        //float intensity = 0.25f + 0.75f * 0.5f * (sin(glfwGetTime()) + 1.0f);
        //lightColor = intensity * lightColor;



        // light position
        float angle = 20.0f * (float)glfwGetTime();
        glm::mat4 rot = glm::mat4(1.0f);
        rot = glm::translate(rot, glm::vec3(100.0f, 40.0f, 100.0f));
        rot = glm::rotate(rot, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 lightPos(rot * glm::vec4(75.0f, 1.0f, 0.0f, 1.0f)); 
        glm::vec3 viewSpaceLightPos = glm::vec3(camera.getViewMatrix() * glm::vec4(lightPos, 1.0f));

        // cube representing light
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, lightPos);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    
        lightsourceShader.use();
        lightsourceShader.setVec3v("lightSourceColor", redLight);
        lightsourceShader.setMat4("view", camera.getViewMatrix());
        lightsourceShader.setMat4("projection", projection);
        lightsourceShader.setMat4("model", modelMatrix);
        crateModel.draw(lightsourceShader);


        // lit object (material)
        lightShader.use();

        // position of light
        lightShader.setVec3v("light.position", viewSpaceLightPos);

        glm::vec3 ambientColor = lightColor * glm::vec3(0.1f); 
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.8f); 
        glm::vec3 specularColor = lightColor * glm::vec3(1.0f); 
        glm::vec3 ambientRed = redLight * glm::vec3(0.1f); 
        glm::vec3 diffuseRed = redLight * glm::vec3(0.8f); 
        glm::vec3 specularRed = redLight * glm::vec3(1.0f); 
        float attenuationConstant = 1.0f;
        float attenuationLinear = 0.014f;
        float attenuationQuadratic = 0.0007f;

        lightShader.setVec3v("light.ambient", ambientColor);
        lightShader.setVec3v("light.diffuse", diffuseColor);
        lightShader.setVec3v("light.specular", specularColor);
        lightShader.setFloat("light.constant", attenuationConstant);
        lightShader.setFloat("light.linear", attenuationLinear);
        lightShader.setFloat("light.quadratic", attenuationQuadratic);	

        // materials of lit object
        lightShader.setVec3("material.ambient", 0.0f, 1.0f, 0.6f);
        lightShader.setVec3("material.diffuse", 0.0f, 1.0f, 1.0);
        lightShader.setVec3("material.specular", 0.5, 0.5, 0.5);
        lightShader.setFloat("material.shininess", 32.0f);

        // transformations
        lightShader.setMat4("view", camera.getViewMatrix());
        lightShader.setMat4("projection", projection);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(99.0f, 38.5f, 99.0f));
        lightShader.setMat4("model", modelMatrix);
        crateModel.draw(lightShader);


        glCheckError();
        // lit object (map)
        lightingShader.use();
        glCheckError();

        // position of light

        glm::vec3 viewSpacePlayerPosition(camera.getViewMatrix() * glm::vec4(camera.getPlayerPOVPosition(), 1.0f));
        glm::mat3 normalMatrix(glm::transpose(glm::inverse(camera.getViewMatrix())));
        glm::vec3 viewSpacePlayerFront(normalMatrix * camera.getPlayerPOVFront());

        glm::vec3 direction(-2.0f, -1.0f, -0.0f);
        glm::vec3 viewSpaceDirection(normalMatrix * direction);
        lightingShader.setVec3v("directionalLight.direction", viewSpaceDirection);
        lightingShader.setVec3v("directionalLight.ambient", 0.2f * ambientColor);
        lightingShader.setVec3v("directionalLight.diffuse", 0.2f * diffuseColor);
        lightingShader.setVec3v("directionalLight.specular", 0.2f * specularColor);

        lightingShader.setVec3v("pointLights[0].position", viewSpaceLightPos);
        lightingShader.setVec3v("pointLights[0].ambient", ambientRed);
        lightingShader.setVec3v("pointLights[0].diffuse", diffuseRed);
        lightingShader.setVec3v("pointLights[0].specular", specularRed);
        lightingShader.setFloat("pointLights[0].constant", attenuationConstant);
        lightingShader.setFloat("pointLights[0].linear", attenuationLinear);
        lightingShader.setFloat("pointLights[0].quadratic", attenuationQuadratic);	

        lightingShader.setVec3v("spotLight.position", viewSpacePlayerPosition);
        lightingShader.setVec3v("spotLight.direction", viewSpacePlayerFront);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
        if (flashlight) {
            lightingShader.setFloat("spotLight.enabled", 1.0f);
        } else {
            lightingShader.setFloat("spotLight.enabled", 0.0f);
        }
        lightingShader.setVec3v("spotLight.ambient", ambientColor);
        lightingShader.setVec3v("spotLight.diffuse", diffuseColor);
        lightingShader.setVec3v("spotLight.specular", specularColor);
        lightingShader.setFloat("spotLight.constant", attenuationConstant);
        lightingShader.setFloat("spotLight.linear", attenuationLinear);
        lightingShader.setFloat("spotLight.quadratic", attenuationQuadratic);	

        glCheckError();

        // materials of lit object
        lightingShader.setVec3("material.specular", 0.5, 0.5, 0.5);
        lightingShader.setFloat("material.shininess", 32.0f);

        glCheckError();

        // transformations
        lightingShader.setMat4("view", camera.getViewMatrix());
        lightingShader.setMat4("projection", projection);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(100.0f, 42.0f, 100.0f));
        lightingShader.setMat4("model", modelMatrix);
        crateModel.draw(lightingShader);


        playerObject.draw(lightingShader);

        mapObject.draw(lightingShader);

        for (unsigned int i = 0; i < 10; ++i) {
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(102.0f, 41.0f, 102.0f));
            float angle = 20.0f * i + 50.0f * (float)glfwGetTime();
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            lightingShader.setMat4("model", modelMatrix);

            crateModel.draw(lightingShader);

        }


        glCheckError();



        glCheckError();

        // swap buffers
        glfwSwapBuffers(window);

        // check and call events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

