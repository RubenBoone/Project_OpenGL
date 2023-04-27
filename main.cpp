#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "FileReader.h"
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main()
{
    // glfw initialize
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Maze project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad load all OpenGL funnction pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader shader(FileReader("resources/shaders/cubeShader.vs").getFileContent(),
        FileReader("resources/shaders/cubeShader.fs").getFileContent());

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    unsigned int vbo, vao;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glUseProgram(shader.ID);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // swap buffers & check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	return 0;
}

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}