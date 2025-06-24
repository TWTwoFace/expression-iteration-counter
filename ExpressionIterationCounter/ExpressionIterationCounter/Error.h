#pragma once
#include <string>
#include "ErrorType.h"

//!Класс, представляющий ошибку
class Error
{
private:
	ErrorType m_type;			//!<Тип ошибки
	std::string m_description;	//!<Описание ошибки
	std::string m_fileTrace;	//!<Трейс ошибки

public:
	/*! Конструктор
	    \param[in] type - тип ошибки
		\param[in] description - описание ошибки
		\param[in] fileTrace - трейсОшибки 
	*/
	Error(ErrorType type, std::string description, std::string fileTrace = "");
	/*! Получить тип ошибки
		\return - тип ошибки
	*/
	ErrorType GetType() const;
	/*! Получить сообщение ошибки
		\return - строка, содержащая преобразованный текст ошибки для вывода
	*/
	std::string GetMessage();
};

