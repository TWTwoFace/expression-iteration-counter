#include "Operation.h"

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

int Operation::GetIterationsCount()
{
    return 0;
}
