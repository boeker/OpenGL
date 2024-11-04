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

#include "texture.h"

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

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && mixFactor < 1.0f) {
        mixFactor += 0.01f;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && mixFactor > 0.0f) {
        mixFactor -= 0.01f;
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

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.movementSpeed = 15.0f;
    } else {
        camera.movementSpeed = 5.0f;
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!camera.falling) {
            camera.falling = true;
            camera.velocity = -9.0f;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.falling = true;
        camera.velocity += -0.22f;
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

    unsigned int VAO; // vertex attribute object
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO; // vertex buffer object
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // vertices forming two triangle forming a rectangle
    //float vertices[] = {
    //     // positions         // texture coords 
    //     0.5f,  0.5f,  0.0f,  1.0f,  1.0f, // top right front
    //     0.5f, -0.5f,  0.0f,  1.0f,  0.0f, // bottom right front
    //    -0.5f, -0.5f,  0.0f,  0.0f,  0.0f, // bottom left front
    //    -0.5f,  0.5f,  0.0f,  0.0f,  1.0f  // top left front
    //};
    float vertices[] = {
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //unsigned int EBO; // element buffer object
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //unsigned int indices[] = {
    //    0, 1, 3, // first triangle
    //    1, 2, 3  // second triangle
    //};
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // tell OpenGL how to interpret our vertex data
    // location of position vertex attribute is 0, consists of 3 values of type GL_FLOAT
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int VAOMap; // vertex attribute object
    glGenVertexArrays(1, &VAOMap);
    glBindVertexArray(VAOMap);

    unsigned int VBOMap; // vertex buffer object
    glGenBuffers(1, &VBOMap);
    glBindBuffer(GL_ARRAY_BUFFER, VBOMap);

    WorldMap myWorldMap;
    myWorldMap.generateMap();

    //int mapSize = 30;
    //float map[] = {
    //     0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    //     1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    //     0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    //     //
    //     1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    //     0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    //     1.0f, -0.2f, -1.0f,  1.0f, 1.0f,
    //};
    int mapSize = myWorldMap.generateVertexList();
    float *map = myWorldMap.vertexList;
    std::cout << mapSize << std::endl;
    glBufferData(GL_ARRAY_BUFFER, mapSize * sizeof(float), map, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Texture wallTexture;
    wallTexture.loadFromFile("textures/wall.jpg");

    Texture containerTexture;
    containerTexture.loadFromFile("textures/container.jpg");

    Texture groundTexture;
    groundTexture.loadFromFile("textures/moon2.jpg");

    glEnable(GL_DEPTH_TEST);

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

    Game game;
    game.draw();
    
    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input
        processInput(window);

        // rendering
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f); // state-setting function
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // state-using function

        // activate program object
        shader.use();
        glUniform1i(glGetUniformLocation(shader.programID, "texture1"), 0);
        glUniform1i(glGetUniformLocation(shader.programID, "texture2"), 1);
        //glUniform1i(glGetUniformLocation(shader.programID, "texture2"), 1);

        //float timeValue = glfwGetTime();
        //shader.setFloat("offsetX", sin(timeValue) / 2.0f);
        //shader.setFloat("offsetY", cos(timeValue) / 2.0f);

        float currentHeight = myWorldMap.getHeight(camera.position.x, camera.position.z);

        if (camera.position.y > currentHeight + 1.8f && !camera.falling) {
            camera.falling = true;
            camera.velocity = 0.0f;
        }

        //camera.processGravity(deltaTime);
        //if (camera.position.y < currentHeight + 1.8f) {
        //    camera.falling = false;
        //    camera.velocity = 0.0f;
        //    camera.position.y = currentHeight + 1.8f;
        //}

        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.fov), 1280.0f / 720.0f, 0.1f, 300.0f);
        glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        

        glActiveTexture(GL_TEXTURE0);
        wallTexture.bind();
    
        // activate texture unit
        glActiveTexture(GL_TEXTURE1);
        // bind texture to currently activated texture unit
        containerTexture.bind();
        

        glUniform1f(glGetUniformLocation(shader.programID, "mixFactor"), mixFactor);

        // bind vertex array object
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + 50.0f * (float)glfwGetTime();
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);

            // draw triangle
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glActiveTexture(GL_TEXTURE1);
        groundTexture.bind();

        glBindVertexArray(VAOMap);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.8f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, mapSize / 5);


        // swap buffers
        glfwSwapBuffers(window);

        // check and call events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

