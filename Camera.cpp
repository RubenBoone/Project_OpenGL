#include "Camera.h"

#include <iostream>

/*
*   Inspired by LearnOpenGL.com
*/

Camera::Camera(float screenWidth, float screenHeight) {
    lastX = screenWidth / 2;
    lastY = screenHeight / 2;
    firstMouse = true;
    
}

void Camera::handleCameraControls(GLFWwindow* window, float deltaTime) 
{

    float cameraSpeed = 2.5f * deltaTime;
    float gravity = -2.5f;
    float jumpAcceleration = 10.0f;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraSpeed = 4.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += cameraSpeed * lookingDirection;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= cameraSpeed * lookingDirection;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= glm::normalize(glm::cross(lookingDirection, UpVector)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += glm::normalize(glm::cross(lookingDirection, UpVector)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        if (position.y < 1.0f && !jumped)
        {
        std::cout << "jump\n";
        position.y += jumpAcceleration * deltaTime;
        }
        else {
            jumped = true;
        }
    }

    if (position.y > 0.0f)
    {
        position.y += gravity * deltaTime;
    }
    if (position.y < 0.0f)
    {
        position.y = 0.0f;
        jumped = false;
    }

}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    lookingDirection = glm::normalize(direction);
}

glm::mat4 Camera::getCamView() {
    return glm::lookAt(position, position + lookingDirection, UpVector);
}