#ifndef SHADER

#include <glad/glad.h>
#include <iostream>

#endif

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
				std::cout << "ERROR::SHADER::" << type << "LINKING_GAILED\n" << infoLog << std::endl;
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
};

