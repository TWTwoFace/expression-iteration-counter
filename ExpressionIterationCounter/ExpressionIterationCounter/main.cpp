#include <iostream>
#include <filesystem>
#include <fstream>
#include <stack>
#include "Logger.h"
#include "ValueType.h"
#include "Validators.h"
#include "Operation.h"
#include "Utils.h"

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

	std::vector<std::string> fileName = SplitString(argv[1], "\\");

	// Output full filepath
	const std::string OutputFilePath = OutputDirectoryPath + SLASH + fileName[fileName.size() - 1] + ".out.txt";

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

	std::set<std::string> variables;
	std::set<Operator> operators;

	Operation* head = NULL;

	std::map<std::string, ValueType> variablesTypesCorrepondes;
	std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;

	if (isTreeFileRead && ValidateTreeFile(treeFileData, logger))
	{
		std::vector<std::string> tokens = SplitString(treeFileData[0], " ");
		
		std::stack<std::pair<ValueType, std::string>> operandsStack;
		std::stack<Operation*> operationsStack;

		for (std::string token : tokens)
		{
			if (IsOperator(token))
			{
				Operator _operator = GetOperatorByToken(token);
				Operation* operation = new Operation(_operator);

				operators.insert(_operator);

				if (IsUnaryOperator(_operator))
				{
					if (operandsStack.size() >= 1)
					{
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
					}
					else
					{
						operation->rightChild = operationsStack.top();
						operation->leftChild = operationsStack.top();
						operationsStack.pop();
					}
				}
				else
				{
					if (operandsStack.size() >= 2)
					{
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
					}
					else if (operandsStack.size() == 1)
					{
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
						operation->rightChild = operationsStack.top();
						operationsStack.pop();
					}
					else
					{
						operation->rightChild = operationsStack.top();
						operationsStack.pop();
						operation->leftChild = operationsStack.top();
						operationsStack.pop();
					}
				}
				operationsStack.push(operation);
			}
			else
			{
				ValueType type = GetValueTypeByValue(token);

				if (type == ValueType::None)
					variables.insert(token);

				std::pair<ValueType, std::string> pair;
				pair.first = type;
				pair.second = token;
				operandsStack.push(pair);
			}
		}
		head = operationsStack.top();
	}

	if (isTypeFileRead && ValidateTypesFile(typesFileData, variables, logger))
	{
		for (std::string line : typesFileData)
		{
			std::vector<std::string> tokens = SplitString(line, " ");

			std::string variable = tokens[1];
			ValueType type = GetValueTypeByToken(tokens[0]);

			variablesTypesCorrepondes[variable] = type;
		}
	}

	if (isIterationsFileRead && ValidateIterationsFile(iterationsFileData, operators, logger))
	{
		for (std::string line : iterationsFileData)
		{
			std::vector<std::string> tokens = SplitString(line, " ");

			Operator _operator = GetOperatorByToken(tokens[0]);
			ValueType type = GetValueTypeByToken(tokens[1]);

			int count = std::stoi(tokens[2]);

			iterationCorrespondes[_operator][type] = count;
		}
	}

	std::ofstream file(OutputFilePath);

	if (!file.is_open())
	{
		Error error(ErrorType::FileCouldNotBeCreated, "Невозможно открыть/создать файл по пути Documents\\\'" + std::string(PROGRAM_NAME) + "\'");
		std::cerr << error.GetMessage() << std::endl;
	}

	if (logger.IsEmpty())
	{
		std::set<Operation*> passedNodes;
		int iterationsCount = head->GetIterationsCount(passedNodes, iterationCorrespondes, variablesTypesCorrepondes);

		file << iterationsCount;
	}
	else
	{
		std::vector<Error> *errors = logger.GetErrors();

		for (Error error : *errors)
		{
			file << error.GetMessage() << '\n';
		}
	}

	file.close();

	return 0;
}