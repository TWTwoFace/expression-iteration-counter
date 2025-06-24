#include "Validators.h"
#include <stack>
#include <cstring>
#include <stdexcept>
#include <cmath>
#include "Utils.h"

#pragma warning(disable: 4996)


bool ValidateTreeFile(const std::vector<std::string>& fileData, ErrorLogger& logger)
{
	// Если размер входного вектора равен нулю
	if (fileData.size() == 0)
	{
		// Определим ошибку о пустоте файла с деревом разбора выражения
		Error error(ErrorType::FileIsEmpty, "Файл с деревом пуст", "Tree file");
		// Запишем ошибку в логгер
		logger.LogError(error);
		// Вернем false
		return false;
	}

	// Будем считать, что результат валидации - true
	bool result = true;

	// Будем считать что количество бинарных операторов, унарных операторов и операндов - 0
	int binaryCount = 0;
	int unaryCount = 0;
	int operandCount = 0;

	// Определим стек операндов
	std::stack<std::string> operands;

	// Разобьем первую строку входного вектора по пробелам
	std::vector<std::string> splittedLine = SplitString(fileData[0], " ");

	// Пройдем по каждому токену
	for (auto token = splittedLine.begin(); token != splittedLine.end(); token++)
	{
		// Если токен - оператор
		if (IsOperator(*token) || IsKeyword(*token))
		{
			// Если токен - ключевое слово, но не оператор
			if (IsKeyword(*token) && !IsOperator(*token))
			{
				// Считаем что результат - false
				result = false;
				// Создадим и запишем в логгер ошибку о неподдерживаевом операторе
				Error error(ErrorType::TreeFileHasUndefinedOperators, "Неподдерживаемый оператор " + *token, "Tree file");
				logger.LogError(error);
			}
			// Если токен - "!" или "~" (Унарный) 
			if (*token == "!" || *token == "~")
			{
				// Если количество операндов - 0
				if (operands.empty())
				{
					// Считаем что результат - false
					result = false;
					// Создадим и запишем в логгер ошибку о недостатке операндов
					Error error(ErrorType::TreeFileMissingOperand, "Отстутствует(ют) операнд(ы) перед оператором " + *token, "Tree file");
					logger.LogError(error);
				}
				// Иначе удалим один верхний элемент из стека операндов
				else
				{
					operands.pop();
				}
				// Прибавим к количеству унарных операторов единицу
				unaryCount++;
			}
			// Иначе, перед нами бинарный оператор
			else
			{
				// Если количество операндов в стеке меньше двух
				if (operands.size() < 2)
				{
					// Считаем что результат - false
					result = false;
					// Создадим и запишем в логгер ошибку о недостатке операндов
					Error error(ErrorType::TreeFileMissingOperand, "Отстутствует(ют) операнд(ы) перед оператором " + *token, "Tree file");
					logger.LogError(error);

					// Удалим все операнды из стека
					while (operands.size() != 0)
					{
						operands.pop();
					}
				}
				// Иначе количество операндов валидное
				else
				{
					// Определим два операнда в виде строк и снимем их со стека 
					std::string firstOperand = operands.top();
					operands.pop();
					std::string secondOperand = operands.top();
					operands.pop();

					// Если токен равен "[]"
					if (*token == "[]")
					{
						// Попробуем преобразовать число в unsigned long
						try
						{
							std::stoul(secondOperand);

						}
						// При неудаче
						catch(std::invalid_argument const& ex) 
						{
							// Считаем что результат - false
							result = false;
							// Создадим и запишем в логгер ошибку о невалидности числа как индекса массива
							Error error(ErrorType::TreeFileFloatingValueAsIndex, "Некорректное число как индекс", "Tree file");
							logger.LogError(error);
						}
					}
				}
				// Прибавим к количеству бинарных операторов единицу
				binaryCount++;
			}
			// Добавим операнд в стек
			operands.push(*token);
		}
		// Иначе перед нами операнд
		else
		{
			// Добавим к количеству операндов единицу
			operandCount++;
			// Добавим операнд в стек
			operands.push(*token);
		}
	}

	// Если количество операторов больше единицы
	if (operands.size() > 1)
	{
		// Считаем что результат - false
		result = false;
		// Создадим и запишем в логгер ошибку о недостатке операторов 
		Error error(ErrorType::TreeFileMissingOperator, "Недостает оператора в дереве разбора выражений", "Tree file");
		logger.LogError(error);
	}

	// Если количество строк в файле разбора выражений больше 1
	if (fileData.size() > 1)
	{
		// Считаем, что результат - false
		result = false;
		// Создадим и запишем в логгер ошибку о лишних строках в файле
		Error error(ErrorType::TreeFileHasExtraLines, "В файле дерева разбора выражений пристутствуют лишние строки (должна быть одна)", "Tree file");
		logger.LogError(error);
	}

	// Вернем результат проверки
	return result;
}

bool ValidateTypesFile(const std::vector<std::string>& fileData, const std::set<std::string>& variables, ErrorLogger& logger)
{
	// Считаем что результат проверки - true
	bool result = true;
	
	// Определим вектор, хранящий переменные найденные в файле типов
	std::vector<std::string> typesFileVariables;

	// Пройдем по каждой строке входных данных
	for (std::string line : fileData)
	{
		// Разобьем строку по пробелам
		std::vector<std::string> splittedLine = SplitString(line, " ");

		// Если количество токенов - 0
		if (splittedLine.size() == 0)
		{
			// Считаем что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о невалидности строки
			Error error(ErrorType::TypeFileInvalidLine, "Строка невалидна (Пустая строка)", "Types file");
			logger.LogError(error);

			// Перейдем на следующую строку
			continue;
		}
		
		// Если количество токенов - 1
		if (splittedLine.size() == 1)
		{
			// Считаем, что результат - false
			result = false;

			// Если первый токен - тип
			if (IsType(splittedLine[0]))
			{
				// Создадим и запишем в логгер ошибку о отсутствие перемменной для типа
				Error error(ErrorType::TypeFileMissingVariableName, "Отсутствует имя переменной для типа " + splittedLine[0], "Types file");
				logger.LogError(error);
			}
			// Иначе, если первый токен - валидное имя переменной
			else if (IsValidVariableName(splittedLine[0]))
			{
				// Создадим и запишем в логгер ошибку о отсутствие типа данных для переменной
				Error error(ErrorType::TypeFileMissingTypeVariable, "Отсутствует тип для переменной " + splittedLine[0], "Types file");
				logger.LogError(error);
			}
			// Иначе, создадим и запишем в логгер ошибку о невалидности строки
			else
			{
				Error error(ErrorType::TypeFileInvalidLine, "Строка невалидна (" + splittedLine[0] + "...)", "Types file");
				logger.LogError(error);
			}

			// Перейдем на следующую строку
			continue;
		}

		// Будем считать что первый токен - тип, второй токен - имя переменной
		std::string type = splittedLine[0];
		std::string variable = splittedLine[1];

		// Если тип - невалидный тип данных
		if (!IsType(type))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о неподдерживаемом типе данных
			Error error(ErrorType::TypeFileHasUndefinedType, "Указан неподдерживаемый тип переменной", "Types file");
			logger.LogError(error);
		}

		if (IsConstant(type))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о констанстном имени переменной
			Error error(ErrorType::TypeFileTypeOverlapsConstant, "Тип данных является зарезервированным именем (константой)", "Types file");
			logger.LogError(error);
		}

		// Если имя переменной - константа
		if (IsConstant(variable))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о констанстном типе данных
			Error error(ErrorType::TypeFileVariableNameOverlapsConstant, "Имя переменной является зарезервированным именем (константой)", "Types file");
			logger.LogError(error);
		}

		// Если имя переменной - ключевое слово
		if (IsKeyword(variable) || IsType(variable))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о зарезервированном имени переменной
			Error error(ErrorType::TypeFileVariableNameOverlapsKeyword, "Имя переменной является зарезервированным именем (ключевое слово)", "Types file");
			logger.LogError(error);
		}

		if (IsKeyword(type))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о зарезервированном типе данных
			Error error(ErrorType::TypeFileTypeOverlapsKeyword, "Тип данных является зарезервированным именем (ключевое слово)", "Types file");
			logger.LogError(error);
		}

		// Если имя переменной невалидно
		if (!IsValidVariableName(variable))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о невалидном имени переменной
			Error error(ErrorType::TypeFileHasInvalidVariableName, "Имя переменной задано некорректно (имя должно быть строкой, состоящей из английских букв в любом регистре)", "Types file");
			logger.LogError(error);
		}

		// Если имени переменной нет в переданном векторе существующих переменных в дереве разбора выражения
		if (std::find(variables.begin(), variables.end(), variable) == variables.end())
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем ошибку в логгер о лишних переменных
			Error error(ErrorType::TypeFileHasExtraVariable, "Переданы лишние переменные (" + variable + ")", "Types file");
			logger.LogError(error);
		}

		// Если имя переменной есть в векторе переменных файла типов
		if (std::find(typesFileVariables.begin(), typesFileVariables.end(), variable) != typesFileVariables.end())
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о дублировании переменной
			Error error(ErrorType::TypeFileVariableNameRepeats, "Тип для переменной " + variable + " объявляется более одного раза", "Types file");
			logger.LogError(error);
		}

		// Добавим имя переменной в вектор переменных файла типов
		typesFileVariables.push_back(variable);
	}

	// Пройдем по элементам вектора переменных существующих в дереве разбора выражения 
	for (std::string variable : variables)
	{
		// Если текущего элемента нет в векторе переменных файла типов
		if (std::find(typesFileVariables.begin(), typesFileVariables.end(), variable) == typesFileVariables.end())
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о нехватке переменной с именем
			Error error(ErrorType::TypeFileMissingTypeVariable, "Не передан тип данных для переменной " + variable, "Types file");
			logger.LogError(error);
		}
	}

	// Вернем результат проверки
	return result;
}

bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger)
{
	// Считаем, что результат проверки - true
	bool result = true;

	// Определим вектор операторов в файле итераций
	std::vector<std::string> fileOperators;

	// Пройдем по вектору входных строк
	for (std::string line : fileData)
	{
		// Разобъем вектор на токены через пробел
		std::vector<std::string> splittedLine = SplitString(line, " ");

		// Если количество токенов - 0
		if (splittedLine.size() == 0)
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о невалидности строки
			Error error(ErrorType::OperatorFileInvalidLine, "Строка невалидна (Пустая строка)", "Operator file");
			logger.LogError(error);
			
			// Перейдем на следующую строку
			continue;
		}

		// Если число токенов - 1
		if (splittedLine.size() == 1)
		{
			// Считаем, что результат - false
			result = false;

			// Если первый токен - тип
			if (IsType(splittedLine[0]))
			{
				// Создадим и запишем в логгер ошибку о нехватке оператора
				Error typeError(ErrorType::OperatorFileMissingOperator, "Отстутствует оператор для типа данных " + splittedLine[0], "Operator file");
				logger.LogError(typeError);

				// Создадим и запишем в логгер ошибку о нехватке количества итераций
				Error countError(ErrorType::OperatorFileMissingCount, "Отсутствует количество итераций для типа данных " + splittedLine[0], "Operator file");
				logger.LogError(countError);
			}
			// Иначе, если первый токен - оператор
			else if (IsOperator(splittedLine[0]))
			{
				// Создадим и запишем в логгер ошибку о нехватке типа данных
				Error operatorError(ErrorType::OperatorFileMissingOperatorType, "Отстутствует тип данных для оператора " + splittedLine[0], "Operator file");
				logger.LogError(operatorError);

				// Создадим и запишем в логгер ошибку о нехватке количества итераций
				Error countError(ErrorType::OperatorFileMissingCount, "Отсутствует количество итераций типа данных " + splittedLine[0], "Operator file");
				logger.LogError(countError);
			}
			// Иначе, создадим и запишем в логгер ошибку о невалидности строки
			else
			{
				Error error(ErrorType::OperatorFileInvalidLine, "Строка невалидна (" + splittedLine[0] + "...)", "Operator file");
				logger.LogError(error);
			}

			// Перейдем на следующую строку
			continue;
		}

		// Если количество токенов - 2
		if (splittedLine.size() == 2)
		{
			// Считаем, что результат - false
			result = false;

			// Если первый токен - оператор и второй токен - тип данных
			if (IsOperator(splittedLine[0]) && IsType(splittedLine[1]))
			{
				// Создадим и запишем в логгер ошибку о нехватке количества итераций
				Error error(ErrorType::OperatorFileMissingCount, "Отсутствует количество итераций в строке (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			// Иначе, если первый токен - тип данных
			else if (IsType(splittedLine[0]))
			{
				// Попробуем преобразовать второй токен в число
				try
				{
					double it = std::stod(splittedLine[1]);

					// Если число меньше нуля
					if (it < 0)
					{
						// Создадим и запишем в логгер ошибку о невалидности числа итераций
						Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть > 0 (" + splittedLine[0] + "...)", "Operator file");
						logger.LogError(error);
					}

					// Если число не целое
					if (std::floor(it) != it)
					{
						// Создадим и запишем в логгер ошибку о невалидности числа итераций 
						Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом(" + splittedLine[0] + "...)", "Operator file");
						logger.LogError(error);
					}
				}
				// При неудаче cоздадим и запишем в логгер ошибку о невалидности числа итераций 
				catch (std::invalid_argument& ex)
				{
					Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом больше нуля (" + splittedLine[0] + "...)", "Operator file");
					logger.LogError(error);
				}

				// Создадим и запишем в логгер ошибку о нехватке оператора 
				Error error(ErrorType::OperatorFileMissingOperator, "Отстутсвует оператор в строке (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			else if (IsOperator(splittedLine[0]) && !IsType(splittedLine[0]))
			{
				// Создадим и запишем в логгер ошибку о нехватке типа данных 
				Error error(ErrorType::OperatorFileMissingValueType, "Отстутсвует тип данных в строке (" + splittedLine[0] + " " + splittedLine[1] + "...)", "Operator file");
				logger.LogError(error);
			}
			// Иначе, создадим и запишем в логгер ошибку о невалидности строки
			else
			{
				Error error(ErrorType::OperatorFileInvalidLine, "Строка невалидна (" + splittedLine[0] + "...)", "Operator file");
				logger.LogError(error);
			}

			// Перейдем на следующую строку
			continue;
		}

		// Считаем что первый токен - оператор, второй токен - тип, третий токен - количество итераций
		std::string _operator = splittedLine[0];
		std::string type = splittedLine[1];
		std::string iterations = splittedLine[2];

		// Если оператор невалиден
		if (!IsOperator(_operator))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о неподдерживаемом операторе
			Error error(ErrorType::OperatorFileHasUndefinedOperator, "Передан неподдерживаемый оператор" + _operator, "Operator file");
			logger.LogError(error);
		}

		// Если тип невалиден
		if (!IsType(type))
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем в логгер ошибку о неподдерживаемом типе данных
			Error error(ErrorType::OperatorFileHasUndefinedType, "Передан неподдерживаемый тип данных " + type, "Operator file");
			logger.LogError(error);
		}

		// Попробуем количество итераций преобразовать в число
		try
		{
			double it = std::stod(iterations);

			// Если число меньше 0
			if (it < 0)
			{
				// Считаем, что результат - false
				result = false;
				// Создадим и запишем ошибку о невалидности числа операций
				Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть > 0 (" + _operator + " " + type + "...)", "Operator file");
				logger.LogError(error);
			}
			
			// Если число не целое
			if (std::floor(it) != it)
			{
				// Считаем, что результат - false
				result = false;
				// Создадим и запишем ошибку о невалидности числа операций
				Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом (" + _operator + " " + type + "...)", "Operator file");
				logger.LogError(error);
			}
		}
		// При неудаче
		catch (std::invalid_argument &ex)
		{
			// Считаем, что результат - false
			result = false;
			// Создадим и запишем ошибку о невалидности числа операций
			Error error(ErrorType::OperatorFileHasInvalidCount, "Количество итераций должно быть целым числом больше нуля (" + _operator + " " + type + "...)", "Operator file");
			logger.LogError(error);
		}
	}

	// Вернем результат проверки
	return result;
}