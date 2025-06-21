#include "Operation.h"

Operation::Operation(Operator operatorType, std::map<Operator, std::map<ValueType, int>>* iterationCorrespondes)
{
    expOperator = operatorType;
    iterationsByOperatorCorrespondes = iterationCorrespondes;
}

ValueType Operation::FetchTypes()
{
    return ValueType::Short;
}

ValueType Operation::GetType()
{
    return ValueType::Short;
}

bool Operation::AddOperand(ValueType operandType)
{
    return false;
}

int Operation::GetIterationsCount(std::set<Operation*> passedNodes)
{
    return 0;
}
