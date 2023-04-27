#include "FileReader.h"

FileReader::FileReader(const char* filePath) {
	std::ifstream file;
	std::string line, temp;
	file.open(filePath);
	if (file)
	{
		while (!file.eof())
		{
			std::getline(file, line);
			temp += line + "\n";
		}		
		temp += "\0";
		fileContent = temp.data();
	}
	else
	{
		std::cerr << "Unable to open file" << std::endl;
	}
	file.close();
}
