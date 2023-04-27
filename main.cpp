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
        // Postion              // TextCoord
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 1.0f,
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f,
        0.5f, 0.5f, 0.0f,       1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    unsigned int vbo, vao, ebo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    int width, height, nColChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("resources/textures/grass.jpg", &width, &height, &nColChannels, 0);

    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(bytes);

    glBindTexture(GL_TEXTURE_2D, 0);

    glUseProgram(shader.ID);
    unsigned int texUniform = glGetUniformLocation(shader.ID, "TexCoord");
    glUniform1i(texUniform, 0);

    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glUseProgram(shader.ID);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffers & check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &texture);
    glDeleteProgram(shader.ID);

	return 0;
}

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}