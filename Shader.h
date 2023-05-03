#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <iostream>

class Shader
{
	void errorCheck(unsigned int shaderToCheck, const char* type) {
		int  success;
		char infoLog[512];
		glGetShaderiv(shaderToCheck, GL_COMPILE_STATUS, &success);

		if (type == "PROGRAM")
		{
			glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::" << type << "::LINKING_FAILED\n" << infoLog << std::endl;
			}
		}
		else
		{
			glGetShaderiv(shaderToCheck, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderToCheck, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
			}
		}
	}
public:
	unsigned int ID;

	Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

	void Enable();
	void Disable();
	void CleanUp();
};

#endif

