#include "Logger.h"

void Logger::LogError(Error& error)
{
	m_errors.push_back(error);
}

std::vector<Error>* Logger::GetErrors()
{
	return &m_errors;
}

bool Logger::IsEmpty()
{
	return m_errors.size() == 0;
}
