#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb_image.h"

#include <iostream>
#include <cmath>

#include "shader.h"
#include "camera.h"
#include "worldmap.h"
#include "game.h"
#include "gameobject.h"

#include "texture.h"
#include "model.h"

float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float mixFactor = 0.5f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = 640;
float lastY = 360;
bool firstMouse = true;

Camera camera;

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
    camera.processFOVChange((float)yoffset);
}

void processInput(GLFWwindow *window, GameObject *object) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processMovement(Camera::Direction::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processMovement(Camera::Direction::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processMovement(Camera::Direction::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processMovement(Camera::Direction::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        object->processMovement(GameObject::Direction::FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        object->processMovement(GameObject::Direction::BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        object->processMovement(GameObject::Direction::LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        object->processMovement(GameObject::Direction::RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.movementSpeed = 15.0f;
    } else {
        camera.movementSpeed = 5.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        object->jump();
        camera.processMovement(Camera::Direction::UPWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.processMovement(Camera::Direction::DOWNWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.velocity += -0.22f;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        if (camera.isAttached()) {
            camera.detach();
        } else {
            camera.attach();
        }
    }
}

int main(int argc, char *argv[]) {
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

    Shader shader("shaders/shader.vs", "shaders/shader.fs");


    Texture wallTexture;
    wallTexture.loadFromFile("textures/wall.jpg");

    Texture containerTexture;
    containerTexture.loadFromFile("textures/container.jpg");

    Texture groundTexture;
    groundTexture.loadFromFile("textures/moon2.jpg");

    // generate height map and create model

    WorldMap myWorldMap;
    myWorldMap.generateMap();
    Game game(&myWorldMap);
    

    int mapSize = myWorldMap.generateVertexList();
    float *map = myWorldMap.vertexList;

    Model worldMapModel(groundTexture, &shader);
    worldMapModel.setGeometry(map, mapSize / 5);
    worldMapModel.transferGeometry();

    GameObject worldMapObject(&worldMapModel, &game);

    Model cube(wallTexture, &shader);
    cube.setGeometry(cubeVertices, 36);
    cube.transferGeometry();

    GameObject someCube(&cube, &game);
    someCube.setHeightOffset(0.5f);
    someCube.setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
    camera.attachToPlayer(&someCube);

    Model crate(containerTexture, &shader);
    crate.setGeometry(cubeVertices, 36);
    crate.transferGeometry();

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
        glm::vec3( 98.8f,  38.0f, 98.5f)  
    };

    glEnable(GL_DEPTH_TEST);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input
        processInput(window, &someCube);
        someCube.simulateGravity(deltaTime);

        // rendering
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f); // state-setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using function

        camera.update();
        shader.setMat4("view", camera.getViewMatrix());

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.fov), 1280.0f / 720.0f, 0.1f, 300.0f);
        shader.setMat4("projection", projection);
        

        for (unsigned int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + 50.0f * (float)glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            crate.draw();
        }
        
        someCube.draw();

        worldMapObject.draw();


        // swap buffers
        glfwSwapBuffers(window);

        // check and call events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

