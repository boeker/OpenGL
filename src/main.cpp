#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <stb_image.h>

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

bool keyCPressed = false;
bool keyFPressed = false;

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

void processInput(GLFWwindow *window, Game &game) {
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
            game.flashlight = !game.flashlight;
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

    Game game(&camera);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    // wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input
        processInput(window, game);
        game.simulateGravity(deltaTime);

        glCheckError();

        // rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // state-setting function
        glStencilMask(0xFF); // Enable writing to stencil buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // state-using function
        glStencilMask(0x00); // Disable writing to stencil buffer

        glCheckError();

        game.processGameLogic((float)glfwGetTime());

        game.setUpShaders();

        glCheckError();

        game.draw();

        glCheckError();

        // swap buffers
        glfwSwapBuffers(window);

        // check and call events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

