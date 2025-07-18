#include "Operation.h"
#include "Utils.h"

Operation::Operation(Operator operatorType)
{
    expOperator = operatorType;
}

ValueType Operation::FetchTypes()
{
    // ���� ���������� ��������� �� 2 � �������� ��������, ������ ��� ������ None
    if (operandTypes.size() != 2 && !IsUnaryOperator(expOperator))
        return ValueType::None;

    // ������� �� ������� ��������
    for (int i = 0; i < operandTypes.size(); i++)
    {
        // ���� ��� ������ �������� ����� None
        if (operandTypes[i] == ValueType::None)
        {
            // ������� ��� ������ ��� �������� �� ������������ ���������� � �� ����� ������
            operandTypes[i] = (*m_variableCorrespondes)[variableNames[i]];
        }
    }

    // ������ ��� ������� ��������, ���� �� ������ �� ��������, ��� ��� �������, ����� ������ ��� ������� ��������
    return operandTypes[0] > operandTypes[1] ? operandTypes[0] : operandTypes[1];
}

ValueType Operation::GetType()
{
    // ������ ���������� ���
    return type;
}

bool Operation::AddOperand(ValueType operandType, std::string variableName)
{
    // ���� ���������� ��������� ������ ��� ����� ����, ������ ���� (�� ���������� �������� �������)
    if (operandTypes.size() >= 2)
        return false;
    
    // ������� ��� �������� � ������ ���� ����������
    variableNames.push_back(variableName);

    // ������� ��� �������� � ������ ����� ���������
    operandTypes.push_back(operandType);
    return true;
}

int Operation::GetIterationsCount(std::set<Operation*> &passedNodes, std::map<Operator, std::map<ValueType, int>> &iterationCorrespondes, std::map<std::string, ValueType>& variableCorrespondes, ErrorLogger &logger)
{
    // ������� � ���� ���������� ��� ������� ����
    passedNodes.insert(this);

    // ��������� ������������ ���������� � �� ����� ��� ��������
    m_variableCorrespondes = &variableCorrespondes;

    // �������, ��� ���������� �������� ����� ����
    int iterations = 0;

    // ���� ����� �������� ���� �� NULL � �� ��� � ��������� ���������� ���
    if (leftChild != NULL && passedNodes.find(leftChild) == passedNodes.end())
    {
        // ������� ���������� �������� ��� �������� ���� � �������� � ���������� ��������
        iterations += leftChild->GetIterationsCount(passedNodes, iterationCorrespondes, variableCorrespondes, logger);
        // ������� ������� � ����� �������� ����
        AddOperand(leftChild->GetType());
    }

    // ���� ������ �������� ���� �� NULL � �� ��� � ��������� ���������� ���
    if (rightChild != NULL && passedNodes.find(rightChild) == passedNodes.end())
    {
        // ������� ���������� �������� ��� �������� ���� � �������� � ���������� ��������
        iterations += rightChild->GetIterationsCount(passedNodes, iterationCorrespondes, variableCorrespondes, logger);
        // ������� ������� � ����� �������� ����
        AddOperand(rightChild->GetType());
    }

    // ��������� ��� ������, ��� �������� ���������� ��������
    ValueType operationType = FetchTypes();

    // ������� ���������� �������� ��� ������� �������� �� ������������ ���������� �������� � ���������� �� ���� ������� ��������
    int nodeIterations = iterationCorrespondes[expOperator][operationType];

    // ���� ���������� ���������� �������� �� ���������� (<= 0)
    if (nodeIterations <= 0)
    {
        // ������� � ������ ������ � ������������� ����������� ������������� ���������� ��� �������� ��������� � ����
        Error error(ErrorType::OperatorFileMissingCount, "����������� ����������� ���������� �������� ��� �������� '" + GetOperatorString(expOperator) + "' ���� '" + GetValueTypeString(operationType) + "'", "Operator file");
        logger.LogError(error);
    }

    // ���� �������� ����������
    if (IsLogicalOperator(expOperator))
    {
        // ���������, ��� ��� ������ �������� - bool
        type = ValueType::Bool;
    }
    // �����, ��� ������ ��� ��, ��� ������� ���������� ��������
    else
    {
        type = operationType;
    }

    if (expOperator == Operator::TakingByIndex)
    {
        if (!IsMassiveType(operandTypes[0]) && !IsMassiveType(operandTypes[1]))
        {
            // �������� � ������� � ������ ������ � ���������� ����� ����������
            Error error(ErrorType::TreeFileInvalidMassiveType, "���������� ������ ���� ��������, �.�. �������� �� ������� �� �������", "Tree file");
            logger.LogError(error);
        }
        else
        {
            operandTypes[0] = GetResultTypeByMassiveType(operandTypes[0]);
            operandTypes[1] = GetResultTypeByMassiveType(operandTypes[1]);

            ValueType resultType = FetchTypes();
            type = resultType;
        }
    }

    // �������� � ������ ���������� �������� ���������� �������� ��� ������� ��������
    iterations += nodeIterations;

    // ������ ���������� ��������
    return iterations;
}
