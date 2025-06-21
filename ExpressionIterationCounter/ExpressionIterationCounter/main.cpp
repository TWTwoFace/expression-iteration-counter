#include <iostream>
#include <filesystem>

#pragma warning(disable: 4996)

#define SLASH "\\"
#define PROGRAM_NAME "Expression Iteration Counter"

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