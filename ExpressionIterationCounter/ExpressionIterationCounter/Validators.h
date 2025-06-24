#pragma once
#include <vector>
#include <string>
#include <set>
#include "Logger.h"
#include "Operator.h"

/*! �������, ������������ ������ �� ����� ������� ���������
	\param[in] fileData - ������ ����� �� �����
	\param[in,out] logger - ������, ��� �������� ��������� ������
*/
bool ValidateTreeFile(const std::vector<std::string>& fileData, ErrorLogger& logger);

/*! �������, ������������ ������ �� ����� ����� ����������
	\param[in] fileData - ������ ����� �� �����
	\param[in] variables - ������ ���� ����������, ������������ � ������
	\param[in,out] logger - ������, ��� �������� ��������� ������
*/
bool ValidateTypesFile(const std::vector<std::string>& fileData, const std::set<std::string>& variables, ErrorLogger& logger);

/*! �������, ������������ ������ �� ����� ����������
	\param[in] fileData - ������ ����� �� �����
	\param[in] operators - ������ ����������, ������������ � ������
	\param[in,out] logger - ������, ��� �������� ��������� ������
*/
bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger);