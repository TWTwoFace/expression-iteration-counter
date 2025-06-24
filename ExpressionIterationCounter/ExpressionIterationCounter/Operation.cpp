#include "Operation.h"
#include "Utils.h"

Operation::Operation(Operator operatorType)
{
    expOperator = operatorType;
}

ValueType Operation::FetchTypes()
{
    // Если количество операндов не 2, вернем тип данных None
    if (operandTypes.size() != 2)
        return ValueType::None;

    // Пройдем по каждому операнду
    for (int i = 0; i < operandTypes.size(); i++)
    {
        // Если тип данных операнда равен None
        if (operandTypes[i] == ValueType::None)
        {
            // Получим тип данных для операнда из соответствия переменных и их типов данных
            operandTypes[i] = (*m_variableCorrespondes)[variableNames[i]];
        }
    }

    // Вернем тип первого операнда, если он больше по значению, чем тип второго, иначе вернем тип второго операнда
    return operandTypes[0] > operandTypes[1] ? operandTypes[0] : operandTypes[1];
}

ValueType Operation::GetType()
{
    // Приведем тип
    type = FetchTypes();
    // Вернем полученный тип
    return type;
}

bool Operation::AddOperand(ValueType operandType, std::string variableName)
{
    // Если количество операндов больше или равно двум, вернем ложь (не получилось добавить операнд)
    if (operandTypes.size() >= 2)
        return false;
    
    // Добавим имя операнда в вектор имен переменных
    variableNames.push_back(variableName);

    // Добавим тип операнда в вектор типов операндов
    operandTypes.push_back(operandType);
    return true;
}

int Operation::GetIterationsCount(std::set<Operation*> &passedNodes, std::map<Operator, std::map<ValueType, int>> &iterationCorrespondes, std::map<std::string, ValueType>& variableCorrespondes, ErrorLogger &logger)
{
    // Добавим в стек пройденных нод текущую нода
    passedNodes.insert(this);

    // Определим соответствие переменных и их типов для операции
    m_variableCorrespondes = &variableCorrespondes;

    // Считаем, что количество итераций равно нулю
    int iterations = 0;

    // Если левая дочерняя нода не NULL и ее нет в множестве пройденных нод
    if (leftChild != NULL && passedNodes.find(leftChild) == passedNodes.end())
    {
        // Получим количество итераций для дочерней ноды и прибавим к количеству итераций
        iterations += leftChild->GetIterationsCount(passedNodes, iterationCorrespondes, variableCorrespondes, logger);
        // Добавим операнд с типом дочерней ноды
        AddOperand(leftChild->GetType());
    }

    // Если правая дочерняя нода не NULL и ее нет в множестве пройденных нод
    if (rightChild != NULL && passedNodes.find(rightChild) == passedNodes.end())
    {
        // Получим количество итераций для дочерней ноды и прибавим к количеству итераций
        iterations += rightChild->GetIterationsCount(passedNodes, iterationCorrespondes, variableCorrespondes, logger);
        // Добавим операнд с типом дочерней ноды
        AddOperand(rightChild->GetType());
    }

    // Определим тип текушей операции
    ValueType nodeType = GetType();

    // Получим количество итераций для текущей операции из соответствия количества итераций и операторов по типу текущей операции
    int nodeIterations = iterationCorrespondes[expOperator][nodeType];

    // Если полученное количество итераций не определено (<= 0)
    if (nodeIterations <= 0)
    {
        // Запишем в логгер ошибку о недостаточном определении соотвтетствий операторов для текущего оператора и типа
        Error error(ErrorType::OperatorFileMissingCount, "Отсутствует определение количества итераций для операции '" + GetOperatorString(expOperator) + "' типа '" + GetValueTypeString(GetType()) + "'", "Operator file");
        logger.LogError(error);
    }

    // Прибавим к общему количеству итераций количество итераций для текущей операции
    iterations += nodeIterations;

    // Вернем количество итераций
    return iterations;
}
