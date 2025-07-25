#include "Validators.h"
#include <stack>
#include <cstring>
#include <stdexcept>
#include <cmath>
#include <map>
#include "Utils.h"

#pragma warning(disable: 4996)


bool ValidateTreeFile(const std::vector<std::string>& fileData, std::map<std::string, bool> *isVariablesMassiveCorresponde, ErrorLogger& logger)
{
	// ���� ������ �������� ������� ����� ����
	if (fileData.size() == 0)
	{
		// ��������� ������ � ������� ����� � ������� ������� ���������
		Error error(ErrorType::FileIsEmpty, "���� � ������� ����", "Tree file");
		// ������� ������ � ������
		logger.LogError(error);
		// ������ false
		return false;
	}

	// ����� �������, ��� ��������� ��������� - true
	bool result = true;

	// ����� ������� ��� ���������� �������� ����������, ������� ���������� � ��������� - 0
	int binaryCount = 0;
	int unaryCount = 0;
	int operandCount = 0;

	// ��������� ���� ���������
	std::stack<std::string> operands;

	// �������� ������ ������ �������� ������� �� ��������
	std::vector<std::string> splittedLine = SplitString(fileData[0], " ");

	// ������� �� ������� ������
	for (auto token = splittedLine.begin(); token != splittedLine.end(); token++)
	{
		// ���� ����� - ��������
		if (IsOperator(*token) || IsKeyword(*token))
		{
			// ���� ����� - �������� �����, �� �� ��������
			if (IsKeyword(*token) && !IsOperator(*token))
			{
				// ������� ��� ��������� - false
				result = false;
				// �������� � ������� � ������ ������ � ���������������� ���������
				Error error(ErrorType::TreeFileHasUndefinedOperators, "���������������� �������� " + *token, "Tree file");
				logger.LogError(error);
			}
			// ���� ����� - "!" ��� "~" (�������) 
			if (*token == "!" || *token == "~")
			{
				// ���� ���������� ��������� - 0
				if (operands.empty())
				{
					// ������� ��� ��������� - false
					result = false;
					// �������� � ������� � ������ ������ � ���������� ���������
					Error error(ErrorType::TreeFileMissingOperand, "������������(��) �������(�) ����� ���������� " + *token, "Tree file");
					logger.LogError(error);
				}
				// ����� ������ ���� ������� ������� �� ����� ���������
				else
				{
					operands.pop();
				}
				// �������� � ���������� ������� ���������� �������
				unaryCount++;
			}
			// �����, ����� ���� �������� ��������
			else
			{
				// ���� ���������� ��������� � ����� ������ ����
				if (operands.size() < 2)
				{
					// ������� ��� ��������� - false
					result = false;
					// �������� � ������� � ������ ������ � ���������� ���������
					Error error(ErrorType::TreeFileMissingOperand, "������������(��) �������(�) ����� ���������� " + *token, "Tree file");
					logger.LogError(error);

					// ������ ��� �������� �� �����
					while (operands.size() != 0)
					{
						operands.pop();
					}
				}
				// ����� ���������� ��������� ��������
				else
				{
					// ��������� ��� �������� � ���� ����� � ������ �� �� ����� 
					std::string secondOperand = operands.top();
					operands.pop();
					std::string firstOperand = operands.top();
					operands.pop();

					// ���� ����� ����� "[]"
					if (*token == "[]")
					{
						if (!IsValidVariableName(firstOperand))
						{
							// �������, ��� ��������� - false
							result = false;
							// �������� � ������� � ������ ������ � ���������� ����� ����������
							Error error(ErrorType::TreeFileInvalidMassiveType, "������ ������� ��� ������ �������� �� ������� ������ ���� ���������� ������ �� ����� �������", "Tree file");
							logger.LogError(error);
						}

						// ��������� ������������� ����� � unsigned long
						try
						{
							double doubIndex = std::stod(secondOperand);
							int intIndex = std::stoi(secondOperand);

							if (doubIndex != intIndex)
							{
								// ������� ��� ��������� - false
								result = false;
								// �������� � ������� � ������ ������ � ������������ ����� ��� ������� �������
								Error error(ErrorType::TreeFileInvalidValueAsIndex, "������������ ����� ��� ������ (������ ���� �����������)", "Tree file");
								logger.LogError(error);
							}

							if (intIndex < 0)
							{
								// ������� ��� ��������� - false
								result = false;
								// �������� � ������� � ������ ������ � ������������ ����� ��� ������� �������
								Error error(ErrorType::TreeFileInvalidValueAsIndex, "������������� ����� ��� ������ (������ ���� �����������)", "Tree file");
								logger.LogError(error);
							}

							(*isVariablesMassiveCorresponde)[firstOperand] = true;
						}
						// ��� �������
						catch(std::exception const& ex) 
						{
							// ������� ��� ��������� - false
							result = false;
							// �������� � ������� � ������ ������ � ������������ ����� ��� ������� �������
							Error error(ErrorType::TreeFileInvalidValueAsIndex, "������ ������ ���� ����������� ������, ��� �������� 2147483647", "Tree file");
							logger.LogError(error);
						}
					}
				}
				// �������� � ���������� �������� ���������� �������
				binaryCount++;
			}
			// ������� ������� � ����
			operands.push(*token);
		}
		// ����� ����� ���� �������
		else
		{
			// ���� ��� ������ �������� ���������
			if (GetValueTypeByValue(*token) == ValueType::Invalid)
			{
				// ������� ��� ��������� - false
				result = false;
				// �������� � ������� � ������ ������ � ������������ ����� ��� ������� �������
				Error error(ErrorType::TreeFileInvalidValue, "�������� ������� ������� ����� " + *token, "Tree file");
				logger.LogError(error);
			}
			// ������� � ���������� ��������� �������
			operandCount++;
			// ������� ������� � ����
			operands.push(*token);
		}
	}

	// ���� ���������� ��������� ������ ���������
	if (binaryCount + unaryCount > 100000)
	{
		// �������, ��� ��������� - false
		result = false;
		// �������� � ������� � ������ ������ � ������� ���������� ����������
		Error error(ErrorType::TreeFileHasExtraLines, "������� ������� ���������� ����������, �������� 100000", "Tree file");
		logger.LogError(error);
	}

	// ���� ���������� ���������� ������ �������
	if (operands.size() > 1)
	{
		// ������� ��� ��������� - false
		result = false;
		// �������� � ������� � ������ ������ � ���������� ���������� 
		Error error(ErrorType::TreeFileMissingOperator, "��������� ��������� � ������ ������� ���������", "Tree file");
		logger.LogError(error);
	}

	// ���� ���������� ����� � ����� ������� ��������� ������ 1
	if (fileData.size() > 1)
	{
		// �������, ��� ��������� - false
		result = false;
		// �������� � ������� � ������ ������ � ������ ������� � �����
		Error error(ErrorType::TreeFileHasExtraLines, "� ����� ������ ������� ��������� ������������� ������ ������ (������ ���� ����)", "Tree file");
		logger.LogError(error);
	}

	// ������ ��������� ��������
	return result;
}

bool ValidateTypesFile(const std::vector<std::string>& fileData, const std::set<std::string>& variables, std::map<std::string, bool> &isVariablesMustBeMassive, bool IsTreeFileValid, ErrorLogger& logger)
{
	// ������� ��� ��������� �������� - true
	bool result = true;
	
	// ��������� ������, �������� ���������� ��������� � ����� �����
	std::vector<std::string> typesFileVariables;

	// ������� �� ������ ������ ������� ������
	for (std::string line : fileData)
	{
		// �������� ������ �� ��������
		std::vector<std::string> splittedLine = SplitString(line, " ");

		// ���� ���������� ������� - 0
		if (splittedLine.size() == 0)
		{
			// ������� ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ������������ ������
			Error error(ErrorType::TypeFileInvalidLine, "������ ��������� (������ ������)", "Types file");
			logger.LogError(error);

			// �������� �� ��������� ������
			continue;
		}
		
		// ���� ���������� ������� - 1
		if (splittedLine.size() == 1)
		{
			// �������, ��� ��������� - false
			result = false;

			// ���� ������ ����� - ���
			if (IsType(splittedLine[0]))
			{
				// �������� � ������� � ������ ������ � ���������� ����������� ��� ����
				Error error(ErrorType::TypeFileMissingVariableName, "����������� ��� ���������� ��� ���� " + splittedLine[0], "Types file");
				logger.LogError(error);
			}
			// �����, ���� ������ ����� - �������� ��� ����������
			else if (IsValidVariableName(splittedLine[0]))
			{
				// �������� � ������� � ������ ������ � ���������� ���� ������ ��� ����������
				Error error(ErrorType::TypeFileMissingTypeVariable, "����������� ��� ��� ���������� " + splittedLine[0], "Types file");
				logger.LogError(error);
			}
			// �����, �������� � ������� � ������ ������ � ������������ ������
			else
			{
				Error error(ErrorType::TypeFileInvalidLine, "������ ��������� (" + splittedLine[0] + "...)", "Types file");
				logger.LogError(error);
			}

			// �������� �� ��������� ������
			continue;
		}

		// ����� ������� ��� ������ ����� - ���, ������ ����� - ��� ����������
		std::string type = splittedLine[0];
		std::string variable = splittedLine[1];

		// ���� ��� - ���������� ��� ������
		if (!IsType(type))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ���������������� ���� ������
			Error error(ErrorType::TypeFileHasUndefinedType, "������ ���������������� ��� ����������", "Types file");
			logger.LogError(error);
		}

		if (IsConstant(type))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ������������ ����� ����������
			Error error(ErrorType::TypeFileTypeOverlapsConstant, "��� ������ �������� ����������������� ������ (����������)", "Types file");
			logger.LogError(error);
		}

		// ���� ��� ���������� - ���������
		if (IsConstant(variable))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ������������ ���� ������
			Error error(ErrorType::TypeFileVariableNameOverlapsConstant, "��� ���������� �������� ����������������� ������ (����������)", "Types file");
			logger.LogError(error);
		}

		// ���� ��� ���������� - �������� �����
		if (IsKeyword(variable) || IsType(variable))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ����������������� ����� ����������
			Error error(ErrorType::TypeFileVariableNameOverlapsKeyword, "��� ���������� �������� ����������������� ������ (�������� �����)", "Types file");
			logger.LogError(error);
		}

		if (IsKeyword(type))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ����������������� ���� ������
			Error error(ErrorType::TypeFileTypeOverlapsKeyword, "��� ������ �������� ����������������� ������ (�������� �����)", "Types file");
			logger.LogError(error);
		}

		// ���� ��� ���������� ���������
		if (!IsValidVariableName(variable))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ���������� ����� ����������
			Error error(ErrorType::TypeFileHasInvalidVariableName, "��� ���������� ������ ����������� (��� ������ ���� �������, ��������� �� ���������� ���� � ����� ��������)", "Types file");
			logger.LogError(error);
		}

		if (isVariablesMustBeMassive[variable] == true)
		{
			if (!IsMassiveType(GetValueTypeByToken(type)))
			{
				// �������, ��� ��������� - false
				result = false;
				// �������� � ������� � ������ ������ � ���������� ����� ����������
				Error error(ErrorType::TreeFileInvalidMassiveType, "���������� ������ ���� ��������, �.�. �������� �� ������� �� �������", "Tree file");
				logger.LogError(error);
			}
		}

		// ���� ����� ���������� ��� � ���������� ������� ������������ ���������� � ������ ������� ��������� � ���� ������ �������
		if (std::find(variables.begin(), variables.end(), variable) == variables.end() && IsTreeFileValid)
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� ������ � ������ � ������ ����������
			Error error(ErrorType::TypeFileHasExtraVariable, "�������� ������ ���������� (" + variable + ")", "Types file");
			logger.LogError(error);
		}

		// ���� ��� ���������� ���� � ������� ���������� ����� �����
		if (std::find(typesFileVariables.begin(), typesFileVariables.end(), variable) != typesFileVariables.end())
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ������������ ����������
			Error error(ErrorType::TypeFileVariableNameRepeats, "��� ��� ���������� " + variable + " ����������� ����� ������ ����", "Types file");
			logger.LogError(error);
		}

		// ������� ��� ���������� � ������ ���������� ����� �����
		typesFileVariables.push_back(variable);
	}

	// ������� �� ��������� ������� ���������� ������������ � ������ ������� ��������� 
	for (std::string variable : variables)
	{
		// ���� �������� �������� ��� � ������� ���������� ����� �����
		if (std::find(typesFileVariables.begin(), typesFileVariables.end(), variable) == typesFileVariables.end())
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � �������� ���������� � ������
			Error error(ErrorType::TypeFileMissingTypeVariable, "�� ������� ��� ������ ��� ���������� " + variable, "Types file");
			logger.LogError(error);
		}
	}

	// ������ ��������� ��������
	return result;
}

bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger)
{
	// �������, ��� ��������� �������� - true
	bool result = true;

	// ��������� ������ ���������� � ����� ��������
	std::vector<std::string> fileOperators;

	// ������� �� ������� ������� �����
	for (std::string line : fileData)
	{
		// �������� ������ �� ������ ����� ������
		std::vector<std::string> splittedLine = SplitString(line, " ");

		// ���� ���������� ������� - 0
		if (splittedLine.size() == 0)
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ������������ ������
			Error error(ErrorType::OperatorFileInvalidLine, "������ ��������� (������ ������)", "Operator file");
			logger.LogError(error);

			// �������� �� ��������� ������
			continue;
		}

		// ���� ����� ������� - 1
		if (splittedLine.size() == 1)
		{
			// �������, ��� ��������� - false
			result = false;

			// ���� ������ ����� - ���
			if (IsType(splittedLine[0]))
			{
				// �������� � ������� � ������ ������ � �������� ���������
				Error typeError(ErrorType::OperatorFileMissingOperator, "������������ �������� ��� ���� ������ " + splittedLine[0], "Operator file");
				logger.LogError(typeError);

				// �������� � ������� � ������ ������ � �������� ���������� ��������
				Error countError(ErrorType::OperatorFileMissingCount, "����������� ���������� �������� ��� ���� ������ " + splittedLine[0], "Operator file");
				logger.LogError(countError);
			}
			// �����, ���� ������ ����� - ��������
			else if (IsOperator(splittedLine[0]))
			{
				// �������� � ������� � ������ ������ � �������� ���� ������
				Error operatorError(ErrorType::OperatorFileMissingOperatorType, "������������ ��� ������ ��� ��������� " + splittedLine[0], "Operator file");
				logger.LogError(operatorError);

				// �������� � ������� � ������ ������ � �������� ���������� ��������
				Error countError(ErrorType::OperatorFileMissingCount, "����������� ���������� �������� ���� ������ " + splittedLine[0], "Operator file");
				logger.LogError(countError);
			}
			// �����, �������� � ������� � ������ ������ � ������������ ������
			else
			{
				Error error(ErrorType::OperatorFileInvalidLine, "������ ��������� (" + splittedLine[0] + "...)", "Operator file");
				logger.LogError(error);
			}

			// �������� �� ��������� ������
			continue;
		}

		// ���� ���������� ������� - 2
		if (splittedLine.size() == 2)
		{
			// �������, ��� ��������� - false
			result = false;

			// ���� ������ ����� - �������� � ������ ����� - ��� ������
			if (IsOperator(splittedLine[0]) && IsType(splittedLine[1]))
			{
				// �������� � ������� � ������ ������ � �������� ���������� ��������
				Error error(ErrorType::OperatorFileMissingCount, "����������� ���������� �������� � ������ (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			// �����, ���� ������ ����� - ��� ������
			else if (IsType(splittedLine[0]))
			{
				// ��������� ������������� ������ ����� � �����
				try
				{
					float it = std::stof(splittedLine[1]);
					int intIt = std::stoi(splittedLine[1]);

					// ���� ����� ������ ����
					if (it < 0)
					{
						// �������� � ������� � ������ ������ � ������������ ����� ��������
						Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ���� > 0 (" + splittedLine[0] + "...)", "Operator file");
						logger.LogError(error);
					}

					// ���� ����� �� �����
					if (intIt != it)
					{
						// �������� � ������� � ������ ������ � ������������ ����� �������� 
						Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ���� ����� ������(" + splittedLine[0] + "...)", "Operator file");
						logger.LogError(error);
					}

					// ���� ����� ������ �������������
					if (it > 32768)
					{
						// �������, ��� ��������� - false
						result = false;

						// �������� � ������� ������ � ������������ ����� ��������
						Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ������������� ��������, �������� 32768", "Operator file");
						logger.LogError(error);
					}
				}
				// ��� ������� c������� � ������� � ������ ������ � ������������ ����� �������� 
				catch (std::exception& ex)
				{
					Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ���� ����� ������ ������ ���� (" + splittedLine[0] + "...)", "Operator file");
					logger.LogError(error);
				}

				// �������� � ������� � ������ ������ � �������� ��������� 
				Error error(ErrorType::OperatorFileMissingOperator, "����������� �������� � ������ (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			else if (IsOperator(splittedLine[0]) && !IsType(splittedLine[0]))
			{
				// �������� � ������� � ������ ������ � �������� ���� ������ 
				Error error(ErrorType::OperatorFileMissingValueType, "����������� ��� ������ � ������ (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			// �����, �������� � ������� � ������ ������ � ������������ ������
			else
			{
				Error error(ErrorType::OperatorFileInvalidLine, "������ ��������� (" + splittedLine[0] + "...)", "Operator file");
				logger.LogError(error);
			}

			// �������� �� ��������� ������
			continue;
		}

		// ������� ��� ������ ����� - ��������, ������ ����� - ���, ������ ����� - ���������� ��������
		std::string _operator = splittedLine[0];
		std::string type = splittedLine[1];
		std::string iterations = splittedLine[2];

		// ���� �������� ���������
		if (!IsOperator(_operator))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ���������������� ���������
			Error error(ErrorType::OperatorFileHasUndefinedOperator, "������� ���������������� �������� " + _operator, "Operator file");
			logger.LogError(error);
		}

		// ���� ��� ���������
		if (!IsType(type))
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� � ������ ������ � ���������������� ���� ������
			Error error(ErrorType::OperatorFileHasUndefinedType, "������� ���������������� ��� ������ " + type, "Operator file");
			logger.LogError(error);
		}

		// ��������� ���������� �������� ������������� � �����
		try
		{
			float it = std::stof(iterations);
			int intIt = std::stoi(iterations);
			// ���� ����� ������ 0
			if (it < 0)
			{
				// �������, ��� ��������� - false
				result = false;
				// �������� � ������� ������ � ������������ ����� ��������
				Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ���� > 0 (" + _operator + " " + type + "...)", "Operator file");
				logger.LogError(error);
			}

			// ���� ����� �� �����
			if (intIt != it)
			{
				// �������, ��� ��������� - false
				result = false;
				// �������� � ������� ������ � ������������ ����� ��������
				Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ���� ����� ������ (" + _operator + " " + type + "...)", "Operator file");
				logger.LogError(error);
			}

			// ���� ����� ������ �������������
			if (it > 32768)
			{
				// �������, ��� ��������� - false
				result = false;

				// �������� � ������� ������ � ������������ ����� ��������
				Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ������������� ��������, �������� 32768", "Operator file");
				logger.LogError(error);
			}
		}
		// ��� �������
		catch (std::exception& ex)
		{
			// �������, ��� ��������� - false
			result = false;
			// �������� � ������� ������ � ������������ ����� ��������
			Error error(ErrorType::OperatorFileHasInvalidCount, "���������� �������� ������ ���� ����� ������ ������ ���� (" + _operator + " " + type + "...)", "Operator file");
			logger.LogError(error);
		}
	}

	// ������ ��������� ��������
	return result;
}