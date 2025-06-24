#include "Utils.h"
#include <stdexcept>
#include <iostream>

#pragma warning(disable: 4996)

std::vector<std::string> SplitString(std::string str, const char* delimeter)
{
	// ��������� �������������� ������
	std::vector<std::string> result;

	// ������� ����� ������� ������ ������� ���������� �� ������ ������������
	char* token = std::strtok((char*)str.c_str(), delimeter);

	// ���� ����� �� ����� nullptr
	while (token != nullptr) 
	{
		// ������� ����� � ����� ������
		result.emplace_back(token);
		token = std::strtok(nullptr, delimeter);
	}

	// ������ �������������� ������
	return result;
}

bool IsOperator(std::string token)
{
	// ��������� ������, �������� ��������� � ���� ������
	std::vector<std::string> operators =
	{
		"+", "-", "*", "/",
		"%", ">", "<", ">=",
		"<=", "==", "!=", "!",
		"&&", "||", "&", "|",
		"^", "~", "<<", ">>",
		"[]",
	};

	// ���� ����� ��������� � �������, ������ true, ����� false
	return std::find(operators.begin(), operators.end(), token) != operators.end();
}

bool IsConstant(std::string str)
{
	// ������ true, ���� str ����� "true" ��� "false"
	return str == "true" || str == "false";
}

bool IsType(std::string str)
{
	// ��������� ������, �������� ���� � ���� �����
	std::vector<std::string> types =
	{
		"bool", "short", "int", "long",
		"float", "double", "char", "char[]",
		"short[]", "int[]", "long[]", "float[]",
		"double[]", "bool[]"
	};

	// ������ true, ���� str ��������� � ������� types, ����� false
	return std::find(types.begin(), types.end(), str) != types.end();
}

bool IsKeyword(std::string str)
{
	// ��������� ������, �������� �������� ����� � ���� �����
	std::vector<std::string> keywords =
	{
		"do", "while", "for", "if", "else", "void", "return"
	};

	// ������ true, ���� str �������� ����� ��� ���������� ��� ��������� � �������
	return IsOperator(str) || std::find(keywords.begin(), keywords.end(), str) != keywords.end();
}

bool IsValidVariableName(std::string str)
{
	// ���� ������ ������ 0, ������ false
	if (str.size() == 0)
		return false;

	// ���� ������ ������ ������ �����, ������ false
	if (isdigit(str[0]))
		return false;

	// ��������� ��������� ������ ��� �������� ��� 0
	int beginIndex = 0;
	// ��������� �������� ������ ��� �������� ��� ������ ������
	int endIndex = str.size();

	// ���� ������ ���������� � ������������� � ' � ������ ������ ����� ���
	if (str[0] == '\'' && str[str.size() - 1] == '\'' && str.size() == 3)
	{
		// ��������� ��������� ������ ��� �������� ��� 1
		beginIndex = 1;
		// ��������� �������� ������ �������� ��� ������ ������ - 1
		endIndex = str.size() - 1;
	}

	// ������� �� ������� ������� ������
	for (int i = 0; i < str.size(); i++)
	{
		// ���� ������ - �� ��������� ����� � ������� ��� ������ �������� � �� _, ������ false
		if ((str[i] < 65 || str[i] > 122) && !isdigit(str[i]) && str[i] != '_')
			return false;
	}

	// ������ ������
	return true;
}

Operator GetOperatorByToken(std::string token)
{
	// ������ ��������������� ������� enum �� ������
	if (token == "+")
		return Operator::Add;
	else if (token == "-")
		return Operator::Substract;
	else if (token == "*")
		return Operator::Multiply;
	else if (token == "/")
		return Operator::Divide;
	else if (token == "%")
		return Operator::Modulo;
	else if (token == ">")
		return Operator::More;
	else if (token == "<")
		return Operator::Less;
	else if (token == ">=")
		return Operator::MoreOrEquals;
	else if (token == "<=")
		return Operator::LessOrEquals;
	else if (token == "==")
		return Operator::Equals;
	else if (token == "!=")
		return Operator::Unequals;
	else if (token == "!")
		return Operator::Not;
	else if (token == "&&")
		return Operator::And;
	else if (token == "||")
		return Operator::Or;
	else if (token == "&")
		return Operator::BitwiseAnd;
	else if (token == "|")
		return Operator::BitwiseOr;
	else if (token == "^")
		return Operator::BitwiseXor;
	else if (token == "~")
		return Operator::Inversion;
	else if (token == "<<")
		return Operator::BitwiseLeftShift;
	else if (token == ">>")
		return Operator::BitwiseRightShift;
	else if (token == "[]")
		return Operator::TakingByIndex;
	
	// ���� ������ �� ����� �� ���������, ������� ����������
	throw new std::invalid_argument("Unexpected operator token");
}

ValueType GetValueTypeByValue(std::string token)
{
	// ���� ����� ���������, ������ ��� ������ Bool
	if (IsConstant(token))
		return ValueType::Bool;

	// ���� ������ ������ ������ ', ������ Char
	if (token[0] == '\'')
		return ValueType::Char;

	// ���� � ������ ��� ������� '.'
	if (token.find('.') == std::string::npos)
	{
		// ��������� ������������� ����� � long long ����������
		try
		{
			long long s_l = std::stoll(token);

			// ���� ���������� ����� ��������� � �������� short, ������ Short
			if (s_l <= SHRT_MAX && s_l >= SHRT_MIN)
				return ValueType::Short;
			// ���� ���������� ����� ��������� � �������� int, ������ Int
			if (s_l <= INT_MAX && s_l >= INT_MIN)
				return ValueType::Int;

			// �����, ������ Long
			return ValueType::Long;
		}
		catch (std::out_of_range& ex)
		{
			return ValueType::Invalid;
		}
		// ��� ������� ������ None
		catch (std::invalid_argument& ex)
		{
			return ValueType::None;
		}
	}
	else
	{
		// ��������� ������������� ����� � long double
		try
		{
			long double s_d = std::stold(token);

			// ���� ���������� ����� ��������� � �������� float, ������ Float
			if (s_d <= FLT_MAX && s_d >= FLT_MIN)
				return ValueType::Float;

			// �����, ������ Double
			return ValueType::Double;
		}
		catch (std::out_of_range& ex)
		{
			return ValueType::Invalid;
		}
		// ��� ������� ������ None
		catch (std::invalid_argument& ex)
		{
			return ValueType::None;
		}
	}
}

bool IsUnaryOperator(Operator _operator)
{
	// ���� �������� ��� �������� �������� ��� ���������, ������ true, ����� false
	return _operator == Operator::Inversion || _operator == Operator::Not;
}

ValueType GetValueTypeByToken(std::string token)
{
	// ������ ��������������� ������� enum �� ������
	if (token == "char")
		return ValueType::Char;
	else if (token == "short")
		return ValueType::Short;
	else if (token == "int")
		return ValueType::Int;
	else if (token == "long")
		return ValueType::Long;
	else if (token == "float")
		return ValueType::Float;
	else if (token == "double")
		return ValueType::Double;
	else if (token == "bool[]")
		return ValueType::BoolArray;
	else if (token == "char[]")
		return ValueType::CharArray;
	else if (token == "short[]")
		return ValueType::ShortArray;
	else if (token == "int[]")
		return ValueType::ShortArray;
	else if (token == "long[]")
		return ValueType::LongArray;
	else if (token == "float[]")
		return ValueType::FloatArray;
	else if (token == "double[]")
		return ValueType::DoubleArray;
	else if (token == "bool")
		return ValueType::Bool;

	// �����, ������� ����������
	throw new std::invalid_argument("Unexpected ValueType token");
}

bool IsLogicalOperator(Operator _operator)
{
	return _operator == Operator::More || _operator == Operator::Less || _operator == Operator::MoreOrEquals
		|| _operator == Operator::LessOrEquals || _operator == Operator::Equals || _operator == Operator::Unequals
		|| _operator == Operator::Not || _operator == Operator::And || _operator == Operator::Or;
}

std::string GetValueTypeString(ValueType type)
{
	// ��������� ������, �������� ���� � ���� �����
	std::vector<std::string> types =
	{
		"invalid", "none", "bool", "char", "short", "int", "long",
		"float", "double", "bool[]", "char[]",
		"short[]", "int[]", "long[]", "float[]",
		"double[]"
	};
	
	// ��������� ������ �� ��������� ����
	std::string stringType = types[(int)type];
	
	// ������ ������
	return stringType;
}

std::string GetOperatorString(Operator _operator)
{
	// ��������� ������, �������� ���� ������ � ���� �����
	std::vector<std::string> operators =
	{
		"+", "-", "*", "/",
		"%", ">", "<", ">=",
		"<=", "==", "!=", "!",
		"&&", "||", "&", "|",
		"^", "~", "<<", ">>",
		"[]",
	};

	// ��������� ������ �� ��������� ���������
	std::string stringOperator = operators[(int)_operator];

	// ������ ������
	return stringOperator;
}