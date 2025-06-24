#pragma once
#include <set>
#include <vector>
#include <map>
#include <string>
#include "Operator.h"
#include "ValueType.h"
#include "Logger.h"

//! �����, �������������� ��������
class Operation
{
public:
	Operation* leftChild = NULL;	//!<��������� �� ����� �������� ����
	Operation* rightChild = NULL;	//!<��������� �� ������ �������� ����

private:
	std::vector<ValueType> operandTypes;						//<������, ���������� ���� ���������
	std::vector<std::string> variableNames;						//<������, �������� ����� ����������
	Operator expOperator;										//<�������� ��������
	ValueType type = ValueType::None;							//<��� ������ ��������
	std::map<std::string, ValueType>* m_variableCorrespondes;	//<������������ ����� ������ � ��� ����������

private:
	/*! ���������� ����� �� ��������� ��������
		\return ��� ������ ��� ��������
	*/
	ValueType FetchTypes();

public:
	/*! �����������
		\param[in] operatorType - ��� ���������
	*/
	Operation(Operator operatorType);

	/*! �������� ��� ��������
		\return ��� ������ ��������
	*/
	ValueType GetType();

	/*! �������� ������� ��������
		\param[in] operandType - ��� ������ ��������
		\param[in] variableName - ��� ����������
		\return true - ���� ������� �������� �������, false - � ���� ������
	*/
	bool AddOperand(ValueType operandType, std::string variableName = "");

	/*! �������� ���������� ��������
		\param[in,out] passedNodes - ���������� ����
		\param[in] iterationCorrespondes - �������������� ���������� � ���������� �������� �� ����� ������
		\param[in] variableCorrespondes - ������������ ���� ���������� � ����� ������
		\return ����� �������� ��� ���������� ��������
	*/
	int GetIterationsCount(std::set<Operation*> &passedNodes, std::map<Operator, std::map<ValueType, int>> &iterationCorrespondes, std::map<std::string, ValueType> &variableCorrespondes, ErrorLogger &logger);
};

