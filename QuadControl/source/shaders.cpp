#include <string>
#include <fstream>
#include <iostream>
#include <vector>

std::string readSourceFile(const char *path)
{
	std::string code;

	std::ifstream file_stream(path,std::ios::in);

	if (file_stream.is_open())
	{
		std::string line =	"";
		while (getline(file_stream, line))
			code += "\n" + line;

		file_stream.close();

		return code;

	}
	else
	{
		std::cout << "Failed to open " << path << std::endl;
		return "";
	}
}

