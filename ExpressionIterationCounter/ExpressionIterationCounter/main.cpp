#include <iostream>
#include <filesystem>
#include <fstream>
#include <stack>
#include "Logger.h"
#include "ValueType.h"
#include "Validators.h"
#include "Operation.h"
#include "Utils.h"

#pragma warning(disable: 4996)

#define SLASH "\\"
#define PROGRAM_NAME "Expression Iteration Counter"

bool ReadFile(const std::string path, std::vector<std::string> &fileData, ErrorLogger& logger)
{
	// Открываем файл
	std::fstream file;
	file.open(path);

	// Если не открылся, добавить ошибку в логгер и вернуть ложь
	if (!file.is_open())
	{
		Error error(ErrorType::FileDoesNotExists, "Не удается открыть файл");
		logger.LogError(error);
		return false;
	}

	// Записать строки в вектор
	std::string line;
	
	while (std::getline(file, line))
	{
		fileData.push_back(line);
	}

	// Закрыть файл и вернуть истину (файл прочитался успешно)
	file.close();

	return true;
}

int main(int argc, char* argv[])
{
	// Установить русский язык для консоли
	setlocale(LC_ALL, "Russian");

	// Если переданных аргументов не 4, вывести ошибку в консоль и вернуть код ошибки 1
	if (argc != 4)
	{
		std::cerr << "Невалидное количество аргументов. Их дожно быть 3." << std::endl;
		std::cerr << "Пример: ExpressionIterationCounter.exe tree.txt types.txt operators.txt" << std::endl;
		return 1;
	}

	// Создание директории для выходных файлов
	const std::string OutputDirectoryPath = std::getenv("HOMEDRIVE") + std::string(std::getenv("HOMEPATH")) + SLASH + "Documents" + SLASH + PROGRAM_NAME;
	std::filesystem::create_directory(OutputDirectoryPath);

	// Определение полного пути для выходного файла
	std::vector<std::string> fileName = SplitString(argv[1], "\\");
	const std::string OutputFilePath = OutputDirectoryPath + SLASH + fileName[fileName.size() - 1] + ".out.txt";

	// Инициализация логгера
	ErrorLogger logger;

	// Определение строк, хранящих пути до входных файлов
	std::string treeFilePath = argv[1];
	std::string typesFilePath = argv[2];
	std::string iterationsFilePath = argv[3];

	// Определение векторов строк, хранящих содержимое входных файлов
	std::vector<std::string> treeFileData;
	std::vector<std::string> typesFileData;
	std::vector<std::string> iterationsFileData;

	// Чтение входных файлов и проверка, прочитался ли
	bool isTreeFileRead = ReadFile(treeFilePath, treeFileData, logger);
	bool isTypeFileRead = ReadFile(typesFilePath, typesFileData, logger);
	bool isIterationsFileRead = ReadFile(iterationsFilePath, iterationsFileData, logger);

	// Определение вспомогательных множеств, храянящих имена переменных и операторы
	std::set<std::string> variables;
	std::set<Operator> operators;

	// Объявление указателя на корневой элемент дерева операций
	Operation* head = NULL;

	// Определение map'ов, храянящих соответствия имён переменных и их типов, операторов и их количество итераций для типов данных
	std::map<std::string, ValueType> variablesTypesCorrepondes;
	std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;

	// Заполнение дерева операций из входных данных, если файл был прочитан и валиден
	if (isTreeFileRead && ValidateTreeFile(treeFileData, logger))
	{
		// Разбиваем строку на токены через пробел
		std::vector<std::string> tokens = SplitString(treeFileData[0], " ");
		
		std::stack<std::pair<ValueType, std::string>> operandsStack;
		std::stack<Operation*> operationsStack;

		// Для каждого токена
		for (std::string token : tokens)
		{
			// Если токен является оператором
			if (IsOperator(token))
			{
				// Определяем элемент enum'a (оператор) по токену
				Operator _operator = GetOperatorByToken(token);
				// Создаем объект операции
				Operation* operation = new Operation(_operator);

				// Добавляем оператор в стек
				operators.insert(_operator);

				// Если оператор - унарный
				if (IsUnaryOperator(_operator))
				{
					// Если в стеке операндов 1 или более элементов, то достаем один элемент и заполняем операнды операции им
					if (operandsStack.size() >= 1)
					{
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
					}
					// Иначе берем верхний элемент из стека операций и заполняем дочерние ноды текущей операции им
					else
					{
						operation->rightChild = operationsStack.top();
						operation->leftChild = operationsStack.top();
						operationsStack.pop();
					}
				}
				// Иначе операция является бинарной
				else
				{
					// Если количество элементов в стеке операндов 2 или более
					if (operandsStack.size() >= 2)
					{
						// Забираем операнды из стека и заполняем операнды текущей операции ими
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
					}
					// Иначе, если количество элементов в стеке операндов 1
					else if (operandsStack.size() == 1)
					{
						// Берем один элемент из стека операндов и заполняем один операнд текущей операции
						operation->AddOperand(operandsStack.top().first, operandsStack.top().second);
						operandsStack.pop();
						// Берем один элемент из тека операций и определяем его, как правую дочернюю ноду текущей операции
						operation->rightChild = operationsStack.top();
						operationsStack.pop();
					}
					// Иначе взять два элемента из стека операций и определить их, как дочерние ноды текущей операции
					else
					{
						operation->rightChild = operationsStack.top();
						operationsStack.pop();
						operation->leftChild = operationsStack.top();
						operationsStack.pop();
					}
				}
				operationsStack.push(operation);
			}
			// Иначе это операнд
			else
			{
				// Определяем тип операнда
				ValueType type = GetValueTypeByValue(token);

				// Если тип операнда None, значит перед нами переменная
				if (type == ValueType::None)
					variables.insert(token);

				// Определяем пару 
				std::pair<ValueType, std::string> pair;
				pair.first = type;
				pair.second = token;
				operandsStack.push(pair);
			}
		}
		// Определяем, что корень дерева - оставшийся элемент в стеке
		head = operationsStack.top();
	}

	// Заполнение данных о переменных и их типах, если файл был прочитан и он валиден
	if (isTypeFileRead && ValidateTypesFile(typesFileData, variables, logger))
	{
		// Пройдем по каждой строке входных данных
		for (std::string line : typesFileData)
		{
			// Разобьеми строку на токены через пробел
			std::vector<std::string> tokens = SplitString(line, " ");

			// Получен тип данных по первому токену
			ValueType type = GetValueTypeByToken(tokens[0]);
			// Получим имя переменной из второго токена
			std::string variable = tokens[1];

			// Составим соответствие из переменной и типа
			variablesTypesCorrepondes[variable] = type;
		}
	}

	// Заполнение данных о количестве итераций для операторов по типам, если файл был прочитан и он валиден
	if (isIterationsFileRead && ValidateIterationsFile(iterationsFileData, operators, logger))
	{
		// Пройдем по каждой строке входных данных
		for (std::string line : iterationsFileData)
		{
			// Разобьем строку на токены через пробел
			std::vector<std::string> tokens = SplitString(line, " ");

			// Получим тип оператора по первому токену
			Operator _operator = GetOperatorByToken(tokens[0]);
			// Получим тип данных по второму токену
			ValueType type = GetValueTypeByToken(tokens[1]);

			// Получем количество итераций по третьему токену
			int count = std::stoi(tokens[2]);

			// Составим соответсвия операторов и количеству итераций по типам данных
			iterationCorrespondes[_operator][type] = count;
		}
	}

	// Создадим и откроем стрим файла по полному пути выходного файла
	std::ofstream file(OutputFilePath);

	// Если файл не получилось открыть
	if (!file.is_open())
	{
		// Выведем ошибку в консоль о невозможности открыть/создать выходной файл
		Error error(ErrorType::FileCouldNotBeCreated, "Невозможно открыть/создать файл по пути Documents\\\'" + std::string(PROGRAM_NAME) + "\'");
		std::cerr << error.GetMessage() << std::endl;
	}

	// Определим результат валидации и записи данных
	bool result = logger.IsEmpty();

	// Если результат - истина
	if (result)
	{
		// Найдем количество итераций для выполнения выражения
		std::set<Operation*> passedNodes;
		int iterationsCount = head->GetIterationsCount(passedNodes, iterationCorrespondes, variablesTypesCorrepondes, logger);

		// Если после логгер оказался не пустым
		if (!logger.IsEmpty())
			// Считаем, что результат - ложь
			result = false;
		// Иначе запишем в выходной файл ответ
		else
			file << iterationsCount;
	}
	
	// Если результат - ложь
	if (!result)
	{
		// Получим ошибки из логгера
		std::vector<Error> *errors = logger.GetErrors();

		// Пройдем по каждой ошибке и запишем их сообщения в выходной файл
		for (Error error : *errors)
		{
			file << error.GetMessage() << '\n';
		}
	}

	// Закроем стрим выходного файла
	file.close();

	return 0;
}