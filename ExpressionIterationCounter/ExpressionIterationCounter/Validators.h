#pragma once
#include <vector>
#include <string>
#include <set>
#include "Logger.h"
#include "Operator.h"

/*! Функция, валидирующая данные из файла разбора выражения
	\param[in] fileData - вектор строк из файла
	\param[in,out] logger - логгер, для хранения возможных ошибок
*/
bool ValidateTreeFile(const std::vector<std::string>& fileData, ErrorLogger& logger);

/*! Функция, валидирующая данные из файла типов переменных
	\param[in] fileData - вектор строк из файла
	\param[in] variables - вектор имен переменных, содержащихся в дереве
	\param[in,out] logger - логгер, для хранения возможных ошибок
*/
bool ValidateTypesFile(const std::vector<std::string>& fileData, const std::set<std::string>& variables, ErrorLogger& logger);

/*! Функция, валидирующая данные из файла операторов
	\param[in] fileData - вектор строк из файла
	\param[in] operators - вектор операторов, содержащихся в дереве
	\param[in,out] logger - логгер, для хранения возможных ошибок
*/
bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger);