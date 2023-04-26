#include "FileReader.h"

FileReader::FileReader(const char* filePath) {
	std::ifstream file;
	std::string line;
	file.open(filePath);
	if (file)
	{
		while (!file.eof())
		{
			std::getline(file, line);
			fileContent += line + "\n";
		}		
	}
	else
	{
		std::cerr << "Unable to open file: " << filePath << std::endl;
	}
	file.close();
}

