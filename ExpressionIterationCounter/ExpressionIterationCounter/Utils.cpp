#include "Utils.h"

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
