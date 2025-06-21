#pragma once
#include <set>
#include <vector>
#include <map>
#include "Operator.h"
#include "ValueType.h"

class Operation
{
public:
	Operation* leftChild = NULL;
	Operation* rightChild = NULL;

private:
	std::vector<ValueType> operandTypes;
	std::map<Operator, std::map<ValueType, int>>* iterationsByOperatorCorrespondes;
	Operator expOperator;
	ValueType type = ValueType::None;

private:
	ValueType FetchTypes();

public:
	Operation(Operator operatorType, std::map<Operator, std::map<ValueType, int>>* iterationCorrespondes);
	ValueType GetType();
	bool AddOperand(ValueType operandType);
	int GetIterationsCount(std::set<Operation*> passedNodes);
};

