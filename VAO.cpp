#include "VAO.h"


VAO::VAO() {
	glGenVertexArrays(1, &ID);
	glBindVertexArray(ID);
}