#include "Operation.h"

ValueType Operation::FetchTypes()
{
    return ValueType::Short;
}

ValueType Operation::GetType()
{
    return ValueType::Short;
}

bool Operation::AddOperand()
{
    return false;
}

int Operation::GetIterationsCount()
{
    return 0;
}
