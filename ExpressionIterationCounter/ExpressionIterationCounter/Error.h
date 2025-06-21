#pragma once
#include <string>
#include "ErrorType.h"

class Error
{
private:
	ErrorType m_type;
	std::string m_description;
	std::string m_fileTrace;

public:
	Error(ErrorType type, std::string description, std::string fileTrace = "");
	std::string GetMessage();
};

