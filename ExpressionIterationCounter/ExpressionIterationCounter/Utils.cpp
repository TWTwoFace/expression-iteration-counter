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

	int beginIndex = 0;
	int endIndex = str.size();

	if (str[0] == '\'' && str[str.size() - 1] == '\'' && str.size() == 3)
	{
		beginIndex = 1;
		endIndex = str.size() - 1;
	}

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

ValueType GetValueTypeByValue(std::string token)
{
	if (token == "true" || token == "false")
		return ValueType::Bool;

	if (token[0] == '\'')
		return ValueType::Char;

	try
	{
		long long s_l = std::stoll(token);

		if (s_l <= SHRT_MAX && s_l >= SHRT_MIN)
			return ValueType::Short;
		if (s_l <= INT_MAX && s_l >= INT_MIN)
			return ValueType::Int;
		
		return ValueType::Long;
	}
	catch (std::invalid_argument& ex)
	{
		try 
		{
			long double s_d = std::stold(token);

			if (s_d <= FLT_MAX && s_d >= FLT_MIN)
				return ValueType::Float;

			return ValueType::Double;
		}
		catch (std::invalid_argument& ex)
		{
			return ValueType::None;
		}
	}
}

bool IsUnaryOperator(Operator _operator)
{
	return _operator == Operator::Inversion || _operator == Operator::Not;
}

ValueType GetValueTypeByToken(std::string token)
{
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

	throw new std::invalid_argument("Unexpected ValueType token");
}