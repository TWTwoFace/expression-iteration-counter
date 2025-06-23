#include "Utils.h"
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

Operator GetOperatorByToken(std::string token)
{
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
	
	throw new std::invalid_argument("Unexpected operator token");
}