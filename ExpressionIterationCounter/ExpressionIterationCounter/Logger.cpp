#include "Logger.h"

void ErrorLogger::LogError(Error& error)
{
	// Добавими ошибку в конец вектора, хранящего ошибки
	m_errors.push_back(error);
}

std::vector<Error>* ErrorLogger::GetErrors()
{
	// Вернем указатель на вектор, храняший ошибки
	return &m_errors;
}

bool ErrorLogger::IsEmpty()
{
	// Вернем true, если размер вектора с ошибка равен нулю, иначе false
	return m_errors.size() == 0;
}
