#pragma once
#include <string>
#include "ErrorType.h"

//!�����, �������������� ������
class Error
{
private:
	ErrorType m_type;			//!<��� ������
	std::string m_description;	//!<�������� ������
	std::string m_fileTrace;	//!<����� ������

public:
	/*! �����������
	    \param[in] type - ��� ������
		\param[in] description - �������� ������
		\param[in] fileTrace - ����������� 
	*/
	Error(ErrorType type, std::string description, std::string fileTrace = "");
	/*! �������� ��� ������
		\return - ��� ������
	*/
	ErrorType GetType() const;
	/*! �������� ��������� ������
		\return - ������, ���������� ��������������� ����� ������ ��� ������
	*/
	std::string GetMessage();
};

