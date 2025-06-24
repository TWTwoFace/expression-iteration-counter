#pragma once
#include <set>
#include <vector>
#include <map>
#include <string>
#include "Operator.h"
#include "ValueType.h"
#include "Logger.h"

//!  ласс, представл€ющий операцию
class Operation
{
public:
	Operation* leftChild = NULL;	//!<”казатель на левую дочернюю ноду
	Operation* rightChild = NULL;	//!<”казатель на правую дочернюю ноду

private:
	std::vector<ValueType> operandTypes;						//<¬ектор, содержащий типы операндов
	std::vector<std::string> variableNames;						//<¬ектор, хран€щий имена переменных
	Operator expOperator;										//<ќператор операции
	ValueType type = ValueType::None;							//<“ип данных операции
	std::map<std::string, ValueType>* m_variableCorrespondes;	//<—оответствие типов данных и имЄн переменных

private:
	/*! ѕриведение типов по операндам операции
		\return тип данных дл€ операции
	*/
	ValueType FetchTypes();

public:
	/*!  онструктор
		\param[in] operatorType - тип оператора
	*/
	Operation(Operator operatorType);

	/*! ѕолучить тип операции
		\return тип данных операции
	*/
	ValueType GetType();

	/*! ƒобавить операнд операции
		\param[in] operandType - тип данных операнда
		\param[in] variableName - им€ переменной
		\return true - если удалось добавить операнд, false - в ином случае
	*/
	bool AddOperand(ValueType operandType, std::string variableName = "");

	/*! ѕолучить количество итераций
		\param[in,out] passedNodes - пройденные ноды
		\param[in] iterationCorrespondes - соотстветстви€ операторов и количества итераций по типам данных
		\param[in] variableCorrespondes - соответстви€ имен переменных и типов данных
		\return число итераций дл€ выполнени€ операции
	*/
	int GetIterationsCount(std::set<Operation*> &passedNodes, std::map<Operator, std::map<ValueType, int>> &iterationCorrespondes, std::map<std::string, ValueType> &variableCorrespondes, ErrorLogger &logger);
};

