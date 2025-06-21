#pragma once
#include <vector>
#include <map>
#include "Operator.h"
#include "ValueType.h"


class Operation
{
public:
	Operation* leftChild;
	Operation* rightChild;
	Operator expOperator;

private:
	std::vector<ValueType> operandTypes;
	std::map<Operator, std::map<ValueType, int>>* iterationsByOperatorCorrespondes;
	ValueType type;

private:
	ValueType FetchTypes();

public:
	ValueType GetType();
	bool AddOperand();
	int GetIterationsCount();
};

