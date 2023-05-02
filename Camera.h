#ifndef CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#endif // !CAMERA


class Camera
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 lookingDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);
	float yaw = -90.0f;
	float pitch = 0.0f;
	bool firstMouse;
	float lastX, lastY;
	bool jumped;

public:
	Camera(float screenWidth, float screenHeight);

	void handleCameraControls(GLFWwindow* window, float deltaTime);

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	glm::mat4 getCamView();
};

