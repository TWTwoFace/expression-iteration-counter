#pragma once
#include <vector>
#include "Error.h"


class Logger
{
private:
	std::vector<Error> m_errors;

public:
	void LogError(Error &error);
	std::vector<Error>* GetErrors();
	bool IsEmpty();
};

