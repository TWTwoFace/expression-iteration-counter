#include "Logger.h"

void ErrorLogger::LogError(Error& error)
{
	m_errors.push_back(error);
}

std::vector<Error>* ErrorLogger::GetErrors()
{
	return &m_errors;
}

bool ErrorLogger::IsEmpty()
{
	return m_errors.size() == 0;
}
