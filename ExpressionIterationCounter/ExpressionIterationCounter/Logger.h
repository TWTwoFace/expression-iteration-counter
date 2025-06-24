#pragma once
#include <vector>
#include "Error.h"

//! �����, �������������� ������
class ErrorLogger
{
private:
	std::vector<Error> m_errors;		//!<����������� ������

public:
	/*! �������� ������
		\param[in] error - ������
	*/
	void LogError(Error &error);

	/*! �������� ������
		\return ������, ����������� �����
	*/
	std::vector<Error>* GetErrors();

	/*! ������ �� ErrorLogger
		\return true - ���� ������ ���, false - � ���� ������
	*/
	bool IsEmpty() const;
};

