#include "Shader.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) 
{
	unsigned int vshader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader_id, 1, &vertexShaderSource, 0);
	glCompileShader(vshader_id);
	errorCheck(vshader_id, "VERTEX");
	
	unsigned int fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader_id, 1, &fragmentShaderSource, 0);
	glCompileShader(fshader_id);
	errorCheck(fshader_id, "FRAGMENT");

	ID = glCreateProgram();
	glAttachShader(ID, vshader_id);
	glAttachShader(ID, fshader_id);
	glLinkProgram(ID);
	errorCheck(ID,"PROGRAM");

	glDeleteShader(vshader_id);
	glDeleteShader(fshader_id);
}

void Shader::Enable()
{
	glUseProgram(ID);
}

void Shader::Disable()
{
	glUseProgram(0);
}

void Shader::Cleanup()
{
	glDeleteProgram(ID);
}
