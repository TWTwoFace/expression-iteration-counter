#include "Error.h"

Error::Error(ErrorType type, std::string description, std::string fileTrace)
{
	m_type = type;
	m_description = description;
	m_fileTrace = fileTrace;
}

std::string Error::GetMessage()
{
	return std::string();
}
