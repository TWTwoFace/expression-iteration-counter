#include "Operation.h"
#include "Utils.h"

Operation::Operation(Operator operatorType)
{
    expOperator = operatorType;
}

ValueType Operation::FetchTypes()
{
    if (operandTypes.size() != 2)
        return ValueType::None;

    for (int i = 0; i < operandTypes.size(); i++)
    {
        if (operandTypes[i] == ValueType::None)
        {
            operandTypes[i] = (*m_variableCorrespondes)[variableNames[i]];
        }
    }

    return operandTypes[0] > operandTypes[1] ? operandTypes[0] : operandTypes[1];
}

ValueType Operation::GetType()
{
    type = FetchTypes();
    return type;
}

bool Operation::AddOperand(ValueType operandType, std::string variableName)
{
    if (operandTypes.size() >= 2)
        return false;

    variableNames.push_back(variableName);

    operandTypes.push_back(operandType);
    return true;
}

int Operation::GetIterationsCount(std::set<Operation*> &passedNodes, std::map<Operator, std::map<ValueType, int>> &iterationCorrespondes, std::map<std::string, ValueType>& variableCorrespondes)
{
    passedNodes.insert(this);
    m_variableCorrespondes = &variableCorrespondes;

    int iterations = 0;

    if (leftChild != NULL && passedNodes.find(leftChild) == passedNodes.end())
    {
        iterations += leftChild->GetIterationsCount(passedNodes, iterationCorrespondes, variableCorrespondes);
        AddOperand(leftChild->GetType());
    }

    if (rightChild != NULL && passedNodes.find(rightChild) == passedNodes.end())
    {
        iterations += rightChild->GetIterationsCount(passedNodes, iterationCorrespondes, variableCorrespondes);
        AddOperand(rightChild->GetType());
    }

    ValueType nodeType = GetType();

    int nodeIterations = iterationCorrespondes[expOperator][nodeType];

    iterations += nodeIterations;

    return iterations;
}
