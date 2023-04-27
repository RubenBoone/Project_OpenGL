#include "Shader.h"

Shader::Shader(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int vshader_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader_id, 1, &vertexShaderSource, 0);
	glCompileShader(vshader_id);
	
	unsigned int fshader_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader_id, 1, &vertexShaderSource, 0);
	glCompileShader(fshader_id);

	ID = glCreateProgram();
	glAttachShader(ID, vshader_id);
	glAttachShader(ID, fshader_id);
	glLinkProgram(ID);
}