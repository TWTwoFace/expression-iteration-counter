#pragma once
#include <vector>
#include "Error.h"

//! Класс, представляющий логгер
class ErrorLogger
{
private:
	std::vector<Error> m_errors;		//!<Добавленные ошибки

public:
	/*! Добавить ошибку
		\param[in] error - ошибка
	*/
	void LogError(Error &error);

	/*! Получить ошибки
		\return ошибки, добавленные ранее
	*/
	std::vector<Error>* GetErrors();

	/*! Пустой ли ErrorLogger
		\return true - если ошибок нет, false - в ином случае
	*/
	bool IsEmpty() const;
};

