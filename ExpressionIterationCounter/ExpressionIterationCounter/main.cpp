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
	// ��������� ����
	std::fstream file;
	file.open(path);

	// ���� �� ��������, �������� ������ � ������ � ������� ����
	if (!file.is_open())
	{
		Error error(ErrorType::FileDoesNotExists, "�� ������� ������� ����");
		logger.LogError(error);
		return false;
	}

	// �������� ������ � ������
	std::string line;
	
	while (std::getline(file, line))
	{
		fileData.push_back(line);
	}

	// ������� ���� � ������� ������ (���� ���������� �������)
	file.close();

	return true;
}

int main(int argc, char* argv[])
{
	system("chcp 65001 > nul");

	// ���� ���������� ���������� �� 4, ������� ������ � ������� � ������� ��� ������ 1
	if (argc != 5)
	{
		std::cout << "���������� ���������� ����������. �� ����� ���� 4." << std::endl;
		std::cout << "������: ExpressionIterationCounter.exe tree.txt types.txt operators.txt output.txt" << std::endl;
		return 1;
	}

	// ������������� �������
	ErrorLogger logger;

	// ����������� �����, �������� ���� �� ������� ������
	const std::string TreeFilePath = argv[1];
	const std::string TypesFilePath = argv[2];
	const std::string IterationsFilePath = argv[3];
	const std::string OutputFilePath = argv[4];

	// ����������� �������� �����, �������� ���������� ������� ������
	std::vector<std::string> treeFileData;
	std::vector<std::string> typesFileData;
	std::vector<std::string> iterationsFileData;

	// ������ ������� ������ � ��������, ���������� ��
	bool isTreeFileRead = ReadFile(TreeFilePath, treeFileData, logger);
	bool isTypeFileRead = ReadFile(TypesFilePath, typesFileData, logger);
	bool isIterationsFileRead = ReadFile(IterationsFilePath, iterationsFileData, logger);

	// ����������� ��������������� ��������, ��������� ����� ���������� � ���������
	std::set<std::string> variables;
	std::set<Operator> operators;

	// ���������� ��������� �� �������� ������� ������ ��������
	Operation* head = NULL;

	// ����������� map'��, ��������� ������������ ��� ���������� � �� �����, ���������� � �� ���������� �������� ��� ����� ������
	std::map<std::string, ValueType> variablesTypesCorrepondes;
	std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
	std::map<std::string, bool> isVariablesMassiveCorrespondes;

	// �������� ����� ������ �� ����������
	bool isTreeFileValid = false;
	
	if (isTreeFileRead)
		isTreeFileValid = ValidateTreeFile(treeFileData, &isVariablesMassiveCorrespondes, logger);

	// ���������� ������ �������� �� ������� ������, ���� ���� ��� �������� � �������
	if (isTreeFileRead && isTreeFileValid)
	{
		// ��������� ������ �� ������ ����� ������
		std::vector<std::string> tokens = SplitString(treeFileData[0], " ");
		
		std::stack<std::pair<ValueType, std::string>> operandsStack;
		std::stack<Operation*> operationsStack;

		// ��� ������� ������
		for (std::string token : tokens)
		{
			// ���� ����� �������� ����������
			if (IsOperator(token))
			{
				// ���������� ������� enum'a (��������) �� ������
				Operator _operator = GetOperatorByToken(token);
				// ������� ������ ��������
				Operation* operation = new Operation(_operator);

				// ��������� �������� � ����
				operators.insert(_operator);

				// ���� �������� - �������
				if (IsUnaryOperator(_operator))
				{
					// ���� � ����� ��������� 1 ��� ����� ���������, �� ������� ���� ������� � ��������� �������� �������� ��
					if (operandsStack.size() >= 1)
					{
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
					}
					// ����� ����� ������� ������� �� ����� �������� � ��������� �������� ���� ������� �������� ��
					else
					{
						operation->rightChild = operationsStack.top();
						operation->AddOperand(ValueType::None);
						operationsStack.pop();
					}
				}
				// ����� �������� �������� ��������
				else
				{
					// ���� ���������� ��������� � ����� ��������� 2 ��� �����
					if (operandsStack.size() >= 2)
					{
						// �������� �������� �� ����� � ��������� �������� ������� �������� ���
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
					}
					// �����, ���� ���������� ��������� � ����� ��������� 1
					else if (operandsStack.size() == 1)
					{
						// ����� ���� ������� �� ����� ��������� � ��������� ���� ������� ������� ��������
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
						// ����� ���� ������� �� ���� �������� � ���������� ���, ��� ������ �������� ���� ������� ��������
						operation->rightChild = operationsStack.top();
						operationsStack.pop();
					}
					// ����� ����� ��� �������� �� ����� �������� � ���������� ��, ��� �������� ���� ������� ��������
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
			// ����� ��� �������
			else
			{
				// ���������� ��� ��������
				ValueType type = GetValueTypeByValue(token);

				// ���� ��� �������� None, ������ ����� ���� ����������
				if (type == ValueType::None)
					variables.insert(token);

				// ���������� ���� 
				std::pair<ValueType, std::string> pair;
				pair.first = type;
				pair.second = token;
				operandsStack.push(pair);
			}
		}
		// ����������, ��� ������ ������ - ���������� ������� � �����
		head = operationsStack.top();
	}

	// ���������� ������ � ���������� � �� �����, ���� ���� ��� �������� � �� �������
	if (isTypeFileRead && ValidateTypesFile(typesFileData, variables, isVariablesMassiveCorrespondes, isTreeFileValid, logger))
	{
		// ������� �� ������ ������ ������� ������
		for (std::string line : typesFileData)
		{
			// ��������� ������ �� ������ ����� ������
			std::vector<std::string> tokens = SplitString(line, " ");

			// ������� ��� ������ �� ������� ������
			ValueType type = GetValueTypeByToken(tokens[0]);
			// ������� ��� ���������� �� ������� ������
			std::string variable = tokens[1];

			// �������� ������������ �� ���������� � ����
			variablesTypesCorrepondes[variable] = type;
		}
	}

	// ���������� ������ � ���������� �������� ��� ���������� �� �����, ���� ���� ��� �������� � �� �������
	if (isIterationsFileRead && ValidateIterationsFile(iterationsFileData, operators, logger))
	{
		// ������� �� ������ ������ ������� ������
		for (std::string line : iterationsFileData)
		{
			// �������� ������ �� ������ ����� ������
			std::vector<std::string> tokens = SplitString(line, " ");

			// ������� ��� ��������� �� ������� ������
			Operator _operator = GetOperatorByToken(tokens[0]);
			// ������� ��� ������ �� ������� ������
			ValueType type = GetValueTypeByToken(tokens[1]);

			// ������� ���������� �������� �� �������� ������
			int count = std::stoi(tokens[2]);

			// �������� ����������� ���������� � ���������� �������� �� ����� ������
			iterationCorrespondes[_operator][type] = count;
		}
	}

	// �������� � ������� ����� ����� �� ������� ���� ��������� �����
	std::ofstream file(OutputFilePath);

	// ���� ���� �� ���������� �������
	if (!file.is_open())
	{
		// ������� ������ � ������� � ������������� �������/������� �������� ����
		Error error(ErrorType::FileCouldNotBeCreated, "���������� �������/������� ���� �� ���� Documents\\\'" + std::string(PROGRAM_NAME) + "\'");
		std::cerr << error.GetMessage() << std::endl;
	}

	// ��������� ��������� ��������� � ������ ������
	bool result = logger.IsEmpty();

	// ���� ��������� - ������
	if (result)
	{
		// ������ ���������� �������� ��� ���������� ���������
		std::set<Operation*> passedNodes;
		int iterationsCount = head->GetIterationsCount(passedNodes, iterationCorrespondes, variablesTypesCorrepondes, logger);

		// ���� ����� ������ �������� �� ������
		if (!logger.IsEmpty())
			// �������, ��� ��������� - ����
			result = false;
		// ����� ������� � �������� ���� �����
		else
		{
			file << iterationsCount;
			std::cout << "��������� ������� ���������" << std::endl;
		}
	}
	
	// ���� ��������� - ����
	if (!result)
	{
		// ������� ������ �� �������
		std::vector<Error> *errors = logger.GetErrors();

		// ������� �� ������ ������ � ������� �� ��������� � �������� ����
		for (Error error : *errors)
		{
			file << error.GetMessage() << '\n';
		}

		std::cout << "��������� ��������� � ��������" << std::endl;
	}

	// ������� ����� ��������� �����
	file.close();

	return 0;
}