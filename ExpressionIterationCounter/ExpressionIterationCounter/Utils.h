#pragma once
#include <vector>
#include <string>
#include "Operator.h"
#include "ValueType.h"

/*!  Функция, для разбиения строк по разделителю
	\param[in] str - строка для разбиения
	\param[in] delimeter - разделитель
	\return вектор, состоящий из элементов исходной строки
*/
std::vector<std::string> SplitString(std::string str, const char* delimeter);

/*! Функция, для определения является ли токен оператором
	\param[in] token - токен
	\return true - если оператор, false - в ином случае 
*/
bool IsOperator(std::string token);

/*! Функция, для определния является ли токен константой
	\param[in] str - токен
	\return true - если константа, false - в ином случае
*/
bool IsConstant(std::string str);

/*! Функция, для определения является ли токен типом данных
	\param[in] str - токен
	\return true - если тип данных, false - в ином случае
*/
bool IsType(std::string str);

/*! Функция, для определения является ли токен ключевым словом
	\param[in] str - токен
	\return true - если ключевое слово, false - в ином случае
*/
bool IsKeyword(std::string str);

/*! Функция, для определения является ли токен валидным именем переменной
	\param[in] str - токен
	\return true - если валидное имя переменной, false - в ином случае
*/
bool IsValidVariableName(std::string str);

/*! Функция, возвращающая Operator по токену
	\param[in] token - токен
	\return соответствующий элемент enum Operator
*/
Operator GetOperatorByToken(std::string token);

/*! Функция, возвращающая ValueType по значению
	\param[in] value - строчное значение
	\return соответствующий элемент enum ValueType
*/
ValueType GetValueTypeByValue(std::string token);

/*! Функция, для определения является ли оператор унарным
	\param[in] _operator - оператор
	\return true - если унарный, false - в ином случае
*/
bool IsUnaryOperator(Operator _operator);

/*! Функция, возвращающая ValueType по токену
	\param[in] token - токен
	\return соответствующий элемент enum ValueType
*/
ValueType GetValueTypeByToken(std::string token);

/*! Функция, для определения является ли оператор логическим
	\param[in] _operator - оператор
	\return true - если логический оператор, false - в ином случае
*/
bool IsLogicalOperator(Operator _operator);

/*! Функция, возвращающая строчное представление для ValueType
	\param[in] type - элемент ValueType
	\return соответствущую строку для элемента ValueType
*/
std::string GetValueTypeString(ValueType type);

/*! Функция, возвращающая строчное представление для Operator
	\param[in] _operator - элемент Operator
	\return соответствущую строку для элемента Operator
*/
std::string GetOperatorString(Operator _operator);

bool IsMassiveType(ValueType type);

ValueType GetResultTypeByMassiveType(ValueType type);