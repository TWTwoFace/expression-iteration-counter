#include "Operation.h"

Operation::Operation(Operator operatorType)
{
    expOperator = operatorType;
    operandTypes;
}

ValueType Operation::FetchTypes()
{
    if (operandTypes.size() != 2)
        return ValueType::None;

    return operandTypes[0] > operandTypes[1] ? operandTypes[0] : operandTypes[1];
}

ValueType Operation::GetType()
{
    type = FetchTypes();
    return type;
}

bool Operation::AddOperand(ValueType operandType)
{
    if (operandTypes.size() >= 2)
        return false;

    operandTypes.push_back(operandType);
    return true;
}

int Operation::GetIterationsCount(std::set<Operation*> &passedNodes, std::map<Operator, std::map<ValueType, int>> &iterationCorrespondes)
{
    passedNodes.insert(this);

    int iterations = 0;

    if (leftChild != NULL && passedNodes.find(leftChild) == passedNodes.end())
    {
        iterations += leftChild->GetIterationsCount(passedNodes, iterationCorrespondes);
        AddOperand(leftChild->GetType());
    }

    if (rightChild != NULL && passedNodes.find(rightChild) == passedNodes.end())
    {
        iterations += rightChild->GetIterationsCount(passedNodes, iterationCorrespondes);
        AddOperand(rightChild->GetType());
    }

    ValueType nodeType = GetType();

    int nodeIterations = iterationCorrespondes[expOperator][nodeType];

    iterations += nodeIterations;

    return iterations;
}
