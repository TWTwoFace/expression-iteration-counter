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
	setlocale(LC_ALL, "Russian");

	if (argc != 4)
	{
		std::cerr << "Invalid input arguments count. It should be 3." << std::endl;
		std::cerr << "Example: ExpressionIterationCounter.exe tree.txt types.txt operators.txt" << std::endl;
		return 1;
	}

	// Creating base folder for program's output files 
	const std::string OutputDirectoryPath = std::getenv("HOMEDRIVE") + std::string(std::getenv("HOMEPATH")) + SLASH + "Documents" + SLASH + PROGRAM_NAME;
	std::filesystem::create_directory(OutputDirectoryPath);

	// Output full filepath
	const std::string OutputFilePath = OutputDirectoryPath + SLASH + SplitString(argv[1], "\\")[0] + ".out.txt";

	ErrorLogger logger;

	std::string treeFilePath = argv[1];
	std::string typesFilePath = argv[2];
	std::string iterationsFilePath = argv[3];

	std::vector<std::string> treeFileData;
	std::vector<std::string> typesFileData;
	std::vector<std::string> iterationsFileData;

	bool isTreeFileRead = ReadFile(treeFilePath, treeFileData, logger);
	bool isTypeFileRead = ReadFile(typesFilePath, typesFileData, logger);
	bool isIterationsFileRead = ReadFile(iterationsFilePath, iterationsFileData, logger);

	return 0;
}