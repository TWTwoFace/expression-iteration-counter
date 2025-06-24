#include "Utils.h"
#include <stdexcept>
#include <iostream>

#pragma warning(disable: 4996)

std::vector<std::string> SplitString(std::string str, const char* delimeter)
{
	// Определим результирующий вектор
	std::vector<std::string> result;

	// Получим токен получив первый элемент отделенный от строки разделителем
	char* token = std::strtok((char*)str.c_str(), delimeter);

	// Пока токен не равен nullptr
	while (token != nullptr) 
	{
		// Добавим токен в конец строки
		result.emplace_back(token);
		token = std::strtok(nullptr, delimeter);
	}

	// Вернем результирующий вектор
	return result;
}

bool IsOperator(std::string token)
{
	// Определим вектор, хранящий операторы в виде строки
	std::vector<std::string> operators =
	{
		"+", "-", "*", "/",
		"%", ">", "<", ">=",
		"<=", "==", "!=", "!",
		"&&", "||", "&", "|",
		"^", "~", "<<", ">>",
		"[]",
	};

	// Если токен находится в векторе, вернем true, иначе false
	return std::find(operators.begin(), operators.end(), token) != operators.end();
}

bool IsConstant(std::string str)
{
	// Вернем true, если str равен "true" или "false"
	return str == "true" || str == "false";
}

bool IsType(std::string str)
{
	// Определим вектор, хранящий типы в виде строк
	std::vector<std::string> types =
	{
		"bool", "short", "int", "long",
		"float", "double", "char", "char[]",
		"short[]", "int[]", "long[]", "float[]",
		"double[]", "bool[]"
	};

	// Вернем true, если str находится в векторе types, иначе false
	return std::find(types.begin(), types.end(), str) != types.end();
}

bool IsKeyword(std::string str)
{
	// Определим вектор, хранящий ключевые слова в виде строк
	std::vector<std::string> keywords =
	{
		"do", "while", "for", "if", "else", "void", "return"
	};

	// Вернем true, если str является типом или оператором или находится в векторе
	return IsOperator(str) || std::find(keywords.begin(), keywords.end(), str) != keywords.end();
}

bool IsValidVariableName(std::string str)
{
	// Если размер строки 0, вернем false
	if (str.size() == 0)
		return false;

	// Если первый символ строки число, вернем false
	if (isdigit(str[0]))
		return false;

	// Определим начальный индекс для проверки как 0
	int beginIndex = 0;
	// Определим конечный индекс для проверки как размер строки
	int endIndex = str.size();

	// Если строка начинается и заканчивается с ' и размер строки равен трём
	if (str[0] == '\'' && str[str.size() - 1] == '\'' && str.size() == 3)
	{
		// Определим начальный индекс для проверки как 1
		beginIndex = 1;
		// Определим конечный индекс проверки как размер строки - 1
		endIndex = str.size() - 1;
	}

	// Пройдем по каждому символу строки
	for (int i = 0; i < str.size(); i++)
	{
		// Если символ - не латинская буква в верхнем или нижнем регистре и не _, вернем false
		if ((str[i] < 65 || str[i] > 122) && !isdigit(str[i]) && str[i] != '_')
			return false;
	}

	// Вернем истину
	return true;
}

Operator GetOperatorByToken(std::string token)
{
	// Вернем соответствующий элемент enum по токену
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
	
	// Если ничего до этого не вернулось, выкенем исключение
	throw new std::invalid_argument("Unexpected operator token");
}

ValueType GetValueTypeByValue(std::string token)
{
	// Если токен константа, вернем тип данных Bool
	if (IsConstant(token))
		return ValueType::Bool;

	// Если первый символ токена ', вернем Char
	if (token[0] == '\'')
		return ValueType::Char;

	// Если в токене нет символа '.'
	if (token.find('.') == std::string::npos)
	{
		// Попробуем преобразовать токен в long long переменную
		try
		{
			long long s_l = std::stoll(token);

			// Если полученное число находится в пределах short, вернем Short
			if (s_l <= SHRT_MAX && s_l >= SHRT_MIN)
				return ValueType::Short;
			// Если полученное число находится в пределах int, вернем Int
			if (s_l <= INT_MAX && s_l >= INT_MIN)
				return ValueType::Int;

			// Иначе, вернем Long
			return ValueType::Long;
		}
		catch (std::out_of_range& ex)
		{
			return ValueType::Invalid;
		}
		// При неудаче вернем None
		catch (std::invalid_argument& ex)
		{
			return ValueType::None;
		}
	}
	else
	{
		// Попробуем преобразовать токен в long double
		try
		{
			long double s_d = std::stold(token);

			// Если полученное число находится в пределах float, вернем Float
			if (s_d <= FLT_MAX && s_d >= FLT_MIN)
				return ValueType::Float;

			// Иначе, вернем Double
			return ValueType::Double;
		}
		catch (std::out_of_range& ex)
		{
			return ValueType::Invalid;
		}
		// При неудаче вернем None
		catch (std::invalid_argument& ex)
		{
			return ValueType::None;
		}
	}
}

bool IsUnaryOperator(Operator _operator)
{
	// Если оператор это оператор инверсии или отрицания, вернем true, иначе false
	return _operator == Operator::Inversion || _operator == Operator::Not;
}

ValueType GetValueTypeByToken(std::string token)
{
	// Вернем соответствующий элемент enum по токену
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

	// Иначе, выкенем исключение
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
	// Определим вектор, хранящий типы в виде строк
	std::vector<std::string> types =
	{
		"invalid", "none", "bool", "char", "short", "int", "long",
		"float", "double", "bool[]", "char[]",
		"short[]", "int[]", "long[]", "float[]",
		"double[]"
	};
	
	// Определим строку по аргументу типа
	std::string stringType = types[(int)type];
	
	// Вернем строку
	return stringType;
}

std::string GetOperatorString(Operator _operator)
{
	// Определим вектор, хранящий типы данных в виде строк
	std::vector<std::string> operators =
	{
		"+", "-", "*", "/",
		"%", ">", "<", ">=",
		"<=", "==", "!=", "!",
		"&&", "||", "&", "|",
		"^", "~", "<<", ">>",
		"[]",
	};

	// Определим строку по аргументу оператора
	std::string stringOperator = operators[(int)_operator];

	// Вернем строку
	return stringOperator;
}