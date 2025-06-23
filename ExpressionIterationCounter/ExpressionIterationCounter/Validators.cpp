#include "Validators.h"
#include <stack>
#include <cstring>
#include <stdexcept>
#include <cmath>
#include "Utils.h"

#pragma warning(disable: 4996)


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
	bool result = true;

	std::vector<std::string> fileOperators;

	for (std::string line : fileData)
	{
		std::vector<std::string> splittedLine = SplitString(line, " ");

		if (splittedLine.size() == 0)
		{
			result = false;
			Error error(ErrorType::OperatorFileInvalidLine, "Строка невалидна (Пустая строка)", "Operator file");
			logger.LogError(error);
			
			continue;
		}

		if (splittedLine.size() == 1)
		{
			result = false;

			if (IsType(splittedLine[0]))
			{
				Error typeError(ErrorType::OperatorFileMissingOperatorType, "Отстутствует тип данных для оператора " + splittedLine[0], "Operator file");
				logger.LogError(typeError);

				Error countError(ErrorType::OperatorFileMissingCount, "Отсутствует количество итераций для оператора " + splittedLine[0], "Operator file");
				logger.LogError(countError);
			}
			else if (IsOperator(splittedLine[0]))
			{
				Error operatorError(ErrorType::OperatorFileMissingOperatorType, "Отстутствует оператор для типа данных " + splittedLine[0], "Operator file");
				logger.LogError(operatorError);

				Error countError(ErrorType::OperatorFileMissingCount, "Отсутствует количество итераций типа данных " + splittedLine[0], "Operator file");
				logger.LogError(countError);
			}
			else
			{
				Error error(ErrorType::OperatorFileInvalidLine, "Строка невалидна (" + splittedLine[0] + "...)", "Operator file");
				logger.LogError(error);
			}

			continue;
		}

		if (splittedLine.size() == 2)
		{
			result = false;

			if (IsOperator(splittedLine[0]) && IsType(splittedLine[1]))
			{
				Error error(ErrorType::OperatorFileMissingCount, "Отсутствует количество итераций в строке (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			else if (IsType(splittedLine[0]))
			{
				try
				{
					double it = std::stod(splittedLine[1]);

					if (it < 0)
					{
						Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть > 0 (" + splittedLine[0] + "...)", "Operator file");
						logger.LogError(error);
					}

					if (std::floor(it) != it)
					{
						Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом(" + splittedLine[0] + "...)", "Operator file");
						logger.LogError(error);
					}
				}
				catch (std::invalid_argument& ex)
				{
					Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом больше нуля (" + splittedLine[0] + "...)", "Operator file");
					logger.LogError(error);
				}

				Error error(ErrorType::OperatorFileMissingOperator, "Отстутсвует оператор в строке (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}

			continue;
		}

		std::string _operator = splittedLine[0];
		std::string type = splittedLine[1];
		std::string iterations = splittedLine[2];

		if (!IsOperator(_operator))
		{
			result = false;
			Error error(ErrorType::OperatorFileHasUndefinedOperator, "Передан неподдерживаемый оператор" + _operator, "Operator file");
			logger.LogError(error);
		}

		if (!IsType(type))
		{
			result = false;
			Error error(ErrorType::OperatorFileHasUndefinedType, "Передан неподдерживаемый тип данных " + type, "Operator file");
			logger.LogError(error);
		}

		try
		{
			double it = std::stod(iterations);

			if (it < 0)
			{
				result = false;
				Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть > 0 (" + _operator + " " + type + "...)", "Operator file");
				logger.LogError(error);
			}
			
			if (std::floor(it) != it)
			{
				result = false;
				Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом(" + _operator + " " + type + "...)", "Operator file");
				logger.LogError(error);
			}
		}
		catch (std::invalid_argument &ex)
		{
			result = false;
			Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом больше нуля (" + _operator + " " + type + "...)", "Operator file");
			logger.LogError(error);
		}
	}

	return result;
}