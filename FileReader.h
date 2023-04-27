#pragma once

#include <iostream>
#include <string>
#include <fstream>

class FileReader
{
	std::string fileContent;
public:
	FileReader(const char* filePath);

	const char* getFileContent() {
		return fileContent.data();
	};
};

