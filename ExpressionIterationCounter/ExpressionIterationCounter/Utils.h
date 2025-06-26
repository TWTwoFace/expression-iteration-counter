#pragma once
#include <vector>
#include <string>
#include "Operator.h"
#include "ValueType.h"

/*!  �������, ��� ��������� ����� �� �����������
	\param[in] str - ������ ��� ���������
	\param[in] delimeter - �����������
	\return ������, ��������� �� ��������� �������� ������
*/
std::vector<std::string> SplitString(std::string str, const char* delimeter);

/*! �������, ��� ����������� �������� �� ����� ����������
	\param[in] token - �����
	\return true - ���� ��������, false - � ���� ������ 
*/
bool IsOperator(std::string token);

/*! �������, ��� ���������� �������� �� ����� ����������
	\param[in] str - �����
	\return true - ���� ���������, false - � ���� ������
*/
bool IsConstant(std::string str);

/*! �������, ��� ����������� �������� �� ����� ����� ������
	\param[in] str - �����
	\return true - ���� ��� ������, false - � ���� ������
*/
bool IsType(std::string str);

/*! �������, ��� ����������� �������� �� ����� �������� ������
	\param[in] str - �����
	\return true - ���� �������� �����, false - � ���� ������
*/
bool IsKeyword(std::string str);

/*! �������, ��� ����������� �������� �� ����� �������� ������ ����������
	\param[in] str - �����
	\return true - ���� �������� ��� ����������, false - � ���� ������
*/
bool IsValidVariableName(std::string str);

/*! �������, ������������ Operator �� ������
	\param[in] token - �����
	\return ��������������� ������� enum Operator
*/
Operator GetOperatorByToken(std::string token);

/*! �������, ������������ ValueType �� ��������
	\param[in] value - �������� ��������
	\return ��������������� ������� enum ValueType
*/
ValueType GetValueTypeByValue(std::string token);

/*! �������, ��� ����������� �������� �� �������� �������
	\param[in] _operator - ��������
	\return true - ���� �������, false - � ���� ������
*/
bool IsUnaryOperator(Operator _operator);

/*! �������, ������������ ValueType �� ������
	\param[in] token - �����
	\return ��������������� ������� enum ValueType
*/
ValueType GetValueTypeByToken(std::string token);

/*! �������, ��� ����������� �������� �� �������� ����������
	\param[in] _operator - ��������
	\return true - ���� ���������� ��������, false - � ���� ������
*/
bool IsLogicalOperator(Operator _operator);

/*! �������, ������������ �������� ������������� ��� ValueType
	\param[in] type - ������� ValueType
	\return �������������� ������ ��� �������� ValueType
*/
std::string GetValueTypeString(ValueType type);

/*! �������, ������������ �������� ������������� ��� Operator
	\param[in] _operator - ������� Operator
	\return �������������� ������ ��� �������� Operator
*/
std::string GetOperatorString(Operator _operator);

bool IsMassiveType(ValueType type);

ValueType GetResultTypeByMassiveType(ValueType type);