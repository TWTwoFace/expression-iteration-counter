#include <iostream>
#include <filesystem>

#pragma warning(disable: 4996)

#define SLASH "\\"
#define PROGRAM_NAME "Expression Iteration Counter"

bool ReadFile(const std::string path, std::vector<std::string> &fileData, ErrorLogger& logger)
{
	std::fstream file;
	file.open(path);

	if (!file.is_open())
	{
		Error error(ErrorType::FileDoesNotExists, "Не удается открыть файл");
		logger.LogError(error);
		return false;
	}

	std::string line;

	while (std::getline(file, line))
	{
		fileData.push_back(line);
	}

	file.close();

	return true;
}

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		std::cout << "Invalid input arguments count. It should be 3." << std::endl;
		std::cout << "Example: ExpressionIterationCounter.exe tree.txt types.txt operators.txt" << std::endl;
		return 0;
	}

	// Creating base folder for program's output files 
	const std::string OutputDirectoryPath = std::getenv("HOMEDRIVE") + std::string(std::getenv("HOMEPATH")) + SLASH + "Documents" + SLASH + PROGRAM_NAME;
	std::filesystem::create_directory(OutputDirectoryPath);

	// Output full filepath
	const std::string OutputFilePath = OutputDirectoryPath + SLASH + argv[1] + ".out.txt";



	return 0;
}