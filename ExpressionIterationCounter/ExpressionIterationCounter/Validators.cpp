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

bool IsConstant(std::string str)
{
	return str == "true" || str == "false";
}

bool IsType(std::string str)
{
	std::vector<std::string> types =
	{
		"bool", "short", "int", "long",
		"float", "double", "char", "char[]",
		"short[]", "int[]", "long[]", "float[]",
		"double[]"
	};
	return std::find(types.begin(), types.end(), str) != types.end();
}

bool IsKeyword(std::string str)
{
	std::vector<std::string> keywords =
	{
		"do", "while", "for", "if", "else", "void", "return"
	};

	return IsType(str) || IsOperator(str) || std::find(keywords.begin(), keywords.end(), str) != keywords.end();
}

bool IsValidVariableName(std::string str)
{
	if (str.size() == 0)
		return false;

	if (isdigit(str[0]))
		return false;

	for (int i = 0; i < str.size(); i++)
	{
		if ((str[i] < 65 || str[i] > 122) && !isdigit(str[i]) && str[i] != '_')
			return false;
	}

	return true;
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
	bool result = true;

	std::vector<std::string> typesFileVariables;

	for (std::string line : fileData)
	{
		std::vector<std::string> splittedLine = SplitString(line, " ");

		if (splittedLine.size() == 0)
		{
			result = false;
			Error error(ErrorType::TypeFileInvalidLine, "Строка невалидна (Пустая строка)", "Types file");
			logger.LogError(error);

			continue;
		}

		if (splittedLine.size() == 1)
		{
			result = false;

			if (IsType(splittedLine[0]))
			{
				Error error(ErrorType::TypeFileMissingVariableName, "Отсутствует имя переменной для типа " + splittedLine[0], "Types file");
				logger.LogError(error);
			}
			else if (IsValidVariableName(splittedLine[0]))
			{
				Error error(ErrorType::TypeFileMissingTypeVariable, "Отсутствует тип для переменной " + splittedLine[0], "Types file");
				logger.LogError(error);
			}
			else
			{
				Error error(ErrorType::TypeFileInvalidLine, "Строка невалидна (" + splittedLine[0] + "...)", "Types file");
				logger.LogError(error);
			}

			continue;
		}

		std::string type = splittedLine[0];
		std::string variable = splittedLine[1];

		if (!IsType(type))
		{
			result = false;
			Error error(ErrorType::TypeFileHasUndefinedType, "Указан неподдерживаемый тип переменной", "Types file");
			logger.LogError(error);
		}

		if (IsConstant(variable))
		{
			result = false;
			Error error(ErrorType::TypeFileVariableNameOverlapsConstant, "Имя переменной является зарезервированным именем (константой)", "Types file");
			logger.LogError(error);
		}

		if (IsKeyword(variable))
		{
			result = false;
			Error error(ErrorType::TypeFileVariableNameOverlapsKeyword, "Имя переменной является зарезервированным именем (ключевое слово)", "Types file");
			logger.LogError(error);
		}

		if (!IsValidVariableName(variable))
		{
			result = false;
			Error error(ErrorType::TypeFileHasInvalidVariableName, "Имя переменной задано некорректно (имя должно быть строкой, состоящей из английских букв в любом регистре)", "Types file");
			logger.LogError(error);
		}

		if (std::find(variables.begin(), variables.end(), variable) == variables.end())
		{
			result = false;
			Error error(ErrorType::TypeFileHasExtraVariable, "Переданы лишние переменные (" + variable + ")", "Types file");
			logger.LogError(error);
		}

		if (std::find(typesFileVariables.begin(), typesFileVariables.end(), variable) != typesFileVariables.end())
		{
			result = false;
			Error error(ErrorType::TypeFileVariableNameRepeats, "Тип для переменной " + variable + " объявляется более одного раза", "Types file");
			logger.LogError(error);
		}

		typesFileVariables.push_back(variable);
	}

	for (std::string variable : variables)
	{
		if (std::find(typesFileVariables.begin(), typesFileVariables.end(), variable) == typesFileVariables.end())
		{
			result = false;
			Error error(ErrorType::TypeFileMissingVariableName, "Не передан тип данных для переменной " + variable, "Types file");
			logger.LogError(error);
		}
	}

	return result;
}

bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger)
{
	return true;
}