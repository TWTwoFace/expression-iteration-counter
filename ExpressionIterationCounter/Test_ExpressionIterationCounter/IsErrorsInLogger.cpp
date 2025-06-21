#include "pch.h"
#include "IsErrorsInLogger.h"

bool IsErrorsInLogger(const std::vector<ErrorType>& errorTypes, ErrorLogger& logger)
{
	std::vector<Error>* errors = logger.GetErrors();
	std::vector<ErrorType> foundedErrorTypes;

	bool result = true;

	//for (auto i = errors->begin(); i != errors->end(); i++)
	//{
	//	foundedErrorTypes.push_back(i->GetType());
	//}

	//for (auto i = errorTypes.begin(); i != errorTypes.end(); i++)
	//{
	//	if (std::find(foundedErrorTypes.begin(), foundedErrorTypes.end(), i) == foundedErrorTypes.end())
	//		result = false;
	//}

	return result;
}
