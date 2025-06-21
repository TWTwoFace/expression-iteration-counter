#pragma once
#include <vector>
#include "Error.h"

class ErrorLogger
{
private:
	std::vector<Error> m_errors;

public:
	void LogError(Error &error);
	std::vector<Error>* GetErrors();
	bool IsEmpty();
};

