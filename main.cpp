#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <irrklang/irrKlang.h>

#include "FileReader.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Model.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadCubemap(std::vector<std::string> faces);


bool CheckCollision(glm::vec3 object);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

bool firstMouse = true;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera playerCam = Camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 0.0f));

irrklang::ISoundEngine* Jukebox = irrklang::createIrrKlangDevice();


struct MazeLocation
{
    glm::vec3 position;
    bool isWall;
    bool isLight;

    MazeLocation(glm::vec3 position, bool isWall, bool isLight)
    {
        this->position = position;
        this->isWall = isWall;
        this->isLight = isLight;
    }
};

// Cube information
float vertices[] = {
    // Postion            // Normals           // TextCoord
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0, 0,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2, 0,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  2, 2,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0, 2,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0, 0,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2, 0,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  2, 2,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0, 2,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0, 0,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  2, 0,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  2, 2,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0, 2,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0, 0,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  2, 0,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  2, 2,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0, 2,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0, 0,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  2, 0,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  2, 2,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0, 2,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0, 0,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  2, 0,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  2, 2,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0, 2,

};

unsigned int indices[] = {
    // Front
    0, 1, 2,
    0, 2, 3,

    // Back
    4, 5, 6,
    4, 6, 7,

    // Left
    8, 9, 10,
    8, 10, 11,

    // Right
    12, 13, 14,
    12, 14, 15,

    // Bottom
    16, 17, 18,
    16, 18, 19,

    // Top
    20, 21, 22,
    20, 22, 23

};

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


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

    Jukebox->play2D("resources/audio/minecraft_sweden.mp3", true);

    // Read maze TXT file
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
                mazeWalls.push_back(MazeLocation(glm::vec3(x, 0.0f, z), false, false));
                x--;
                break;
            case '#':
                mazeWalls.push_back(MazeLocation(glm::vec3(x, 0.0f, z), true, false));
                x--;
                break;
            case 'x':
                //x needs both torch AND floor
                mazeWalls.push_back(MazeLocation(glm::vec3(x, -0.35f, z), false, true));
                mazeWalls.push_back(MazeLocation(glm::vec3(x, 0.0f, z), false, false));
                x--;
                break;
            case '\n':
                x = 0.0f;
                z -= 1.0f;
                break;
        }
        i++;
    }

    // Creating offset list for instancing
    std::vector<glm::vec3> floorTranslations;
    for (size_t i = 0; i < mazeWalls.size(); i++)
    {
        glm::vec3 translation(mazeWalls[i].position.x, -1.0f, mazeWalls[i].position.z);
        floorTranslations.push_back(translation);
    }

    std::vector<glm::vec3> wallTranslations;
    std::vector<glm::vec3> torchTranslations;
    for (size_t i = 0; i < mazeWalls.size(); i++)
    {
        if (mazeWalls[i].isWall)
        {
            glm::vec3 translation(mazeWalls[i].position.x, mazeWalls[i].position.y, mazeWalls[i].position.z);
            wallTranslations.push_back(translation);
        }
        else if (mazeWalls[i].isLight)
        {
            glm::vec3 translation(mazeWalls[i].position.x, mazeWalls[i].position.y, mazeWalls[i].position.z);
            torchTranslations.push_back(translation);
        }
    }

    // Create shaders
    Shader shader(FileReader("resources/shaders/cubeShader.vs").getFileContent(),
                  FileReader("resources/shaders/cubeShader.fs").getFileContent());

    Shader lightShader(FileReader("resources/shaders/cubeShader.vs").getFileContent(),
                       FileReader("resources/shaders/lightShader.fs").getFileContent());


    Shader skyboxShader(FileReader("resources/shaders/skyboxShader.vs").getFileContent(),
        FileReader("resources/shaders/skyboxShader.fs").getFileContent());
  
    Shader modelShader(FileReader("resources/shaders/modelShader.vs").getFileContent(),
        FileReader("resources/shaders/modelShader.fs").getFileContent());

    // Create VAO, VBO & EBO's
    VAO floorVAO = VAO();
    VBO cubeVBO = VBO(vertices, sizeof(vertices));
    EBO cubeEBO = EBO(indices, sizeof(indices));

    floorVAO.AddAttrib(cubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
    floorVAO.AddAttrib(cubeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    floorVAO.AddAttrib(cubeVBO, 3, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int floorInstanceVBO;
    glGenBuffers(1, &floorInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, floorInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * floorTranslations.size(), &floorTranslations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, floorInstanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    floorVAO.Unbind();
    cubeVBO.UnBind();
    cubeEBO.UnBind();

    VAO wallVAO = VAO();
    cubeVBO.Bind();
    cubeEBO.Bind();

    wallVAO.AddAttrib(cubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
    wallVAO.AddAttrib(cubeVBO, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    wallVAO.AddAttrib(cubeVBO, 3, 2, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    unsigned int wallInstanceVBO;
    glGenBuffers(1, &wallInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, wallInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * wallTranslations.size(), &wallTranslations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, wallInstanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1);

    wallVAO.Unbind();
    cubeVBO.UnBind();
    cubeEBO.UnBind();

    VAO lightVAO = VAO();
    cubeVBO.Bind();
    cubeEBO.Bind();

    lightVAO.AddAttrib(cubeVBO, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);

    lightVAO.Unbind();
    cubeVBO.UnBind();
    cubeEBO.UnBind();

    // Create Textures
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
    skyboxFaces.push_back("resources/skybox/left.jpg");
    skyboxFaces.push_back("resources/skybox/right.jpg");
    skyboxFaces.push_back("resources/skybox/top.jpg");
    skyboxFaces.push_back("resources/skybox/bottom.jpg");
    skyboxFaces.push_back("resources/skybox/front.jpg");
    skyboxFaces.push_back("resources/skybox/back.jpg");

    unsigned int cubemapTexture = loadCubemap(skyboxFaces);

    skyboxShader.Enable();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);

    // Enable GL functions
    glEnable(GL_DEPTH_TEST);
  
  
    //Loads diamond for testing, this can be changed later
    Model diamondModel("resources/models/torch/Torch.obj");
    glm::mat4 model = glm::mat4(1.0f);

    // Draw loop
    while (!glfwWindowShouldClose(window))
    {
        // timing
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        glm::vec3 lastPos = playerCam.Position;
        playerCam.InputHandler(window, deltaTime);
        bool isCollision = CheckCollision(wallTranslations[0]);


        for (size_t i = 0; i < wallTranslations.size(); i++)
        {
            isCollision = CheckCollision(wallTranslations[i]);
            if (isCollision)
            {
                break;
            }
        }

        if (isCollision)
        {
            playerCam.Position.x = lastPos.x;
            playerCam.Position.z = lastPos.z;
        }


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Draw 3d model
        modelShader.Enable(); 

        glm::mat4 view = playerCam.getView(); 
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(playerCam.getProjection())); 
        glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view)); 

        glUniform4f(glGetUniformLocation(modelShader.ID, "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(modelShader.ID, "numOfLights"), torchTranslations.size());
        glUniform3f(glGetUniformLocation(modelShader.ID, "viewPos"), playerCam.Position.x, playerCam.Position.y, playerCam.Position.z);

        glm::mat4 assimpModel = glm::mat4(1.0f);
        for (size_t i = 0; i < torchTranslations.size(); i++)
        {
            assimpModel = glm::mat4(1.0f);
            assimpModel = glm::translate(assimpModel, glm::vec3(torchTranslations[i].x, torchTranslations[i].y, torchTranslations[i].z));
            assimpModel = glm::scale(assimpModel, glm::vec3(0.2f, 0.2f, 0.2f));
            assimpModel = glm::rotate(assimpModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(modelShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(assimpModel));
            std::string index = to_string(i);

            glUniform3f(glGetUniformLocation(modelShader.ID, ("light[" + index + "].position").c_str()), torchTranslations[i].x, torchTranslations[i].y + 1, torchTranslations[i].z);
            glUniform1f(glGetUniformLocation(modelShader.ID, ("light[" + index + "].constant").c_str()), 0.2f);
            glUniform1f(glGetUniformLocation(modelShader.ID, ("light[" + index + "].linear").c_str()), 0.7f);
            glUniform1f(glGetUniformLocation(modelShader.ID, ("light[" + index + "].quadratic").c_str()), 1.8f);
            diamondModel.Draw(modelShader);
        }
      
        // Transform local coordinats to view coordiantes
        shader.Enable();
        model = glm::mat4(1.0f);
        glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), 1.0f, 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shader.ID, "numOfLights"), torchTranslations.size());
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "cameraMatrix"), 1, GL_FALSE, glm::value_ptr(playerCam.getCamMatrix()));
        glUniform3f(glGetUniformLocation(shader.ID, "viewPos"), playerCam.Position.x, playerCam.Position.y, playerCam.Position.z);

        for (size_t i = 0; i < torchTranslations.size(); i++)
        {
            std::string index = to_string(i);

            glUniform3f(glGetUniformLocation(shader.ID, ("light[" + index + "].position").c_str()), torchTranslations[i].x, torchTranslations[i].y, torchTranslations[i].z);
            glUniform1f(glGetUniformLocation(shader.ID, ("light[" + index + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(shader.ID, ("light[" + index + "].linear").c_str()), 0.7f);
            glUniform1f(glGetUniformLocation(shader.ID, ("light[" + index + "].quadratic").c_str()), 1.8f);
        }

        // render
        wallVAO.Bind();
        leaves.Bind();
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, wallTranslations.size());

        floorVAO.Bind();
        gravel.Bind();
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, floorTranslations.size());

        //draw skybox
        glDepthFunc(GL_LEQUAL); 
        skyboxShader.Enable();
        glm::mat4 skyboxView = glm::mat4(glm::mat3(playerCam.getView()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(skyboxView));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(playerCam.getProjection()));

        glBindVertexArray(skyboxVAO); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture); 
        glDrawArrays(GL_TRIANGLES, 0, 36); 
        glBindVertexArray(0); 
        glDepthFunc(GL_LESS);


        // swap buffers & check events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
  
  // Cleanup
    floorVAO.CleanUp();
    cubeVBO.CleanUp();
    wallVAO.CleanUp();
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

bool CheckCollision(glm::vec3 object)
{
    glm::vec3 posBeforeColl = playerCam.Position;
    if (playerCam.Position.x <= object.x + 0.65f &&
        playerCam.Position.y <= object.y + 1.65f &&
        playerCam.Position.z <= object.z + 0.65f &&
        playerCam.Position.x >= object.x - 0.65f &&
        playerCam.Position.y >= object.y - 1.65f &&
        playerCam.Position.z >= object.z - 0.65f )
    {
        std::cout << "Collision!" << std::endl;
        return true;
    }

    return false;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
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
