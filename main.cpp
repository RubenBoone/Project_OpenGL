#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "FileReader.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera playerCam = Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

struct MazeLocation
{
    glm::vec3 position;
    bool isWall;

    MazeLocation(glm::vec3 position, bool isWall) {
        this->position = position;
        this->isWall = isWall;
    }
};

float vertices[] = {
    // Postion            // TextCoord
   -0.5f, -0.5f, -0.5f,  0, 0, // Front Bottom Left
    0.5f, -0.5f, -0.5f,  2, 0, // Front Bottom Right
    0.5f,  0.5f, -0.5f,  2, 2, // Front Top Right
   -0.5f,  0.5f, -0.5f,  0, 2, // Front Top Left

   -0.5f, -0.5f,  0.5f,  2, 0, // Back Bottom Left
    0.5f, -0.5f,  0.5f,  0, 0, // Back Bottom Right
    0.5f,  0.5f,  0.5f,  0, 2, // Back Top Right
   -0.5f,  0.5f,  0.5f,  2, 2, // Back Top Left

   -0.5f, -0.5f, -0.5f,  2, 2, // Front Bottom Left for bottom
    0.5f,  0.5f, -0.5f,  0, 0, // Back Top right for top
};

unsigned int indices[] = {
    // Front
    0, 1, 2,
    0, 2, 3, 

    // Back
    4, 5, 6,
    4, 6, 7,

    // Right
    1, 2, 5,
    2, 5, 6,

    // Left
    0, 4, 3,
    4, 3, 7,

    // Bottom
    8, 1, 5,
    8, 5, 4,

    // Top
    3, 9, 7,
    7, 6, 9,

};

float skyboxVertices[] = {
    // positions          
  -20.0f,  20.0f, -20.0f,
    -20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f,  20.0f, -20.0f,
    -20.0f,  20.0f, -20.0f,

    -20.0f, -20.0f,  20.0f,
    -20.0f, -20.0f, -20.0f,
    -20.0f,  20.0f, -20.0f,
    -20.0f,  20.0f, -20.0f,
    -20.0f,  20.0f,  20.0f,
    -20.0f, -20.0f,  20.0f,

     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,

    -20.0f, -20.0f,  20.0f,
    -20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f, -20.0f,  20.0f,
    -20.0f, -20.0f,  20.0f,

    -20.0f,  20.0f, -20.0f,
     20.0f,  20.0f, -20.0f,
     20.0f,  20.0f,  20.0f,
     20.0f,  20.0f,  20.0f,
    -20.0f,  20.0f,  20.0f,
    -20.0f,  20.0f, -20.0f,

    -20.0f, -20.0f, -20.0f,
    -20.0f, -20.0f,  20.0f,
     20.0f, -20.0f, -20.0f,
     20.0f, -20.0f, -20.0f,
    -20.0f, -20.0f,  20.0f,
     20.0f, -20.0f,  20.0f
};

unsigned int loadCubemap(std::vector<std::string> faces)
{
    std::cout << "in func\n";

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::cout << "in loop\n";
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            std::cout << "In if\n";
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 

    return textureID;
}

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

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad load all OpenGL funnction pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::vector<MazeLocation> mazeWalls;

    std::string maze = FileReader("resources/maze.txt").getFileContent();
    int i = 0;
    float x = 0.0f;
    float z = 0.0f;
    while (maze[i] != '\0')
    {
        switch (maze[i])
        {
        case ' ':
            mazeWalls.push_back(MazeLocation(glm::vec3(x, 0.0f, z), false));
            x--;
            break;
        case '#':
            mazeWalls.push_back(MazeLocation(glm::vec3(x, 0.0f, z), true));
            x--;
            break;
        case '\n':
            x = 0.0f;
            z -= 1.0f;
            break;
        }
        i++;
    }

    glEnable(GL_DEPTH_TEST);

    // TODO: Transparency is a bit buggy if we just do this... Disabled it for now
    // we use transparant texture 
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader shader(FileReader("resources/shaders/cubeShader.vs").getFileContent(),
        FileReader("resources/shaders/cubeShader.fs").getFileContent());


    Shader skyboxShader(FileReader("resources/shaders/skyboxShader.vs").getFileContent(),
        FileReader("resources/shaders/skyboxShader.fs").getFileContent());

    VAO vao = VAO();
    VBO vbo = VBO(vertices, sizeof(vertices));
    EBO ebo = EBO(indices, sizeof(indices));

    vao.AddAttrib(vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
    vao.AddAttrib(vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float)));

    vbo.UnBind();
    vao.Unbind();
    // EBO uBind after VAO

    Texture leaves("resources/textures/leaves.png", GL_TEXTURE_2D, GL_TEXTURE0);
    Texture gravel("resources/textures/gravel.png", GL_TEXTURE_2D, GL_TEXTURE0);


    //Skybox vao, vbo
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);



    std::vector<std::string> skyboxFaces; 
    skyboxFaces.push_back("resources/skybox/right.jpg");
    skyboxFaces.push_back("resources/skybox/left.jpg");
    skyboxFaces.push_back("resources/skybox/top.jpg");
    skyboxFaces.push_back("resources/skybox/bottom.jpg");
    skyboxFaces.push_back("resources/skybox/front.jpg");
    skyboxFaces.push_back("resources/skybox/back.jpg");


    unsigned int cubemapTexture = loadCubemap(skyboxFaces);

    skyboxShader.Enable();  



    while (!glfwWindowShouldClose(window))
    {
        // timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);
        playerCam.InputHandler(window, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
               
        shader.Enable();
        int modelLoc = glGetUniformLocation(shader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(playerCam.getCamMatrix()));

        //draw skybox
        glDepthFunc(GL_LEQUAL);
        //glDepthMask(GL_FALSE);  
        skyboxShader.Enable();
        //TODO: With this view the skybox should move with camera, for one reason or another this doesnt work so skybox is huge
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(playerCam.getView()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(playerCam.getProjection()));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        //glDepthMask(GL_TRUE);  
        glDepthFunc(GL_LESS);

        // render
        vao.Bind();

        for (int i = 0; i < (int)mazeWalls.size(); i++)
        {
            if (mazeWalls[i].isWall)
            {
                leaves.Bind();
                model = glm::mat4(1.0f);
                model = glm::translate(model, mazeWalls[i].position);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            }
            gravel.Bind();
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(mazeWalls[i].position.x, -1.0f, mazeWalls[i].position.z));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        // swap buffers & check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    vao.CleanUp();
    vbo.CleanUp();
    leaves.CleanUp();
    gravel.CleanUp();
    shader.CleanUp();

	return 0;
}

void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    playerCam.Yaw += xoffset;
    playerCam.Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (playerCam.Pitch > 89.0f)
        playerCam.Pitch = 89.0f;
    if (playerCam.Pitch < -89.0f)
        playerCam.Pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(playerCam.Yaw)) * cos(glm::radians(playerCam.Pitch));
    front.y = sin(glm::radians(playerCam.Pitch));
    front.z = sin(glm::radians(playerCam.Yaw)) * cos(glm::radians(playerCam.Pitch));
    playerCam.LookingDirection = glm::normalize(front);
}
