#include "Validators.h"
#include <stack>
#include <cstring>
#include <stdexcept>

#pragma warning(disable: 4996)

std::vector<std::string> SplitString(std::string str, const char* delimeter)
{
	std::vector<std::string> result;
	char* token = std::strtok((char*)str.c_str(), delimeter);

	while (token != nullptr) {
		result.emplace_back(token);
		token = std::strtok(nullptr, delimeter);
	}

	return result;
}

bool IsOperator(std::string token)
{
	std::vector<std::string> operators =
	{
		"+", "-", "*", "/",
		"%", ">", "<", ">=",
		"<=", "==", "!=", "!",
		"&&", "||", "&", "|",
		"^", "~", "<<", ">>",
		"[]",
	};

	return std::find(operators.begin(), operators.end(), token) != operators.end();
}

bool ValidateTreeFile(const std::vector<std::string>& fileData, ErrorLogger& logger)
{
	if (fileData.size() == 0)
	{
		Error error(ErrorType::FileIsEmpty, "TreeFileIsEmpty");
		logger.LogError(error);
		return false;
	}

	bool result = true;

	int binaryCount = 0;
	int unaryCount = 0;
	int operandCount = 0;

	std::stack<std::string> operands;

	std::vector<std::string> splittedLine = SplitString(fileData[0], " ");

	for (auto token = splittedLine.begin(); token != splittedLine.end(); token++)
	{
		if (IsOperator(*token))
		{
			if (*token == "!" || *token == "~")
			{
				if (operands.empty())
				{
					result = false;
					Error error(ErrorType::TreeFileMissingOperand, "tree file missing operand");
					logger.LogError(error);
				}
				else
				{
					operands.pop();
				}
				unaryCount++;
			}
			else
			{
				if (operands.size() < 2)
				{
					result = false;
					Error error(ErrorType::TreeFileMissingOperand, "tree file missing operand");
					logger.LogError(error);

					while (operands.size() != 0)
					{
						operands.pop();
					}
				}
				else
				{
					std::string firstOperand = operands.top();
					operands.pop();
					std::string secondOperand = operands.top();
					operands.pop();

					if (*token == "[]")
					{
						try
						{
							std::stoul(secondOperand);

						}
						catch(std::invalid_argument const& ex) 
						{
							result = false;
							Error error(ErrorType::TreeFileFloatingValueAsIndex, "you try to take value from array by floating index");
							logger.LogError(error);
						}
					}
				}
				binaryCount++;
			}
			operands.push(*token);
		}
		else
		{
			operandCount++;
			operands.push(*token);
		}
	}

	if (operands.size() > 1)
	{
		result = false;
		Error error(ErrorType::TreeFileMissingOperator, "tree file missing operator");
		logger.LogError(error);
	}

	if (fileData.size() > 1)
	{
		result = false;
		Error error(ErrorType::TreeFileHasExtraLines, "you must write you'r expression in one line");
		logger.LogError(error);
	}

	return result;
}

bool ValidateTypesFile(const std::vector<std::string>& fileData, const std::set<std::string>& variables, ErrorLogger& logger)
{
	return true;
}

bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger)
{
	return true;
}