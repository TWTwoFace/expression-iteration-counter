#include "Error.h"
#include <stdexcept>

constexpr const char* ErrorTypeToString(ErrorType e)
{
	// Вернем соответствующую строку для каждой ошибки 
	switch (e)
	{
	case ErrorType::FileIsEmpty:
		return "FileIsEmpty";
	case ErrorType::FileDoesNotExists:
		return "FileDoesNotExists";
	case ErrorType::FileCouldNotBeCreated:
		return "FileCouldNotBeCreated";
	case ErrorType::TreeFileHasExtraLines:
		return "TreeFileHasExtraLines";
	case ErrorType::TreeFileMissingOperator:
		return "TreeFileMissingOperator";
	case ErrorType::TreeFileInvalidMassiveType:
		return "TreeFileInvalidMassiveType";
	case ErrorType::TreeFileHasNotExpression:
		return "TreeFileHasNotExpression";
	case ErrorType::TreeFileHasUndefinedOperators:
		return "TreeFileHasUndefinedOperators";
	case ErrorType::TreeFileMissingOperand:
		return "TreeFileMissingOperand";
	case ErrorType::TreeFileHasExtraOperands:
		return "TreeFileHasExtraOperands";
	case ErrorType::TreeFileInvalidValue:
		return "TreeFileInvalidValue";
	case ErrorType::TreeFileInvalidValueAsIndex:
		return "TreeFileFloatingValuesAsIndex";
	case ErrorType::TypeFileHasUndefinedType:
		return "TypeFileHasUndefinedType";
	case ErrorType::TypeFileMissingTypeVariable:
		return "TypeFileMissingTypeVariable";
	case ErrorType::TypeFileInvalidLine:
		return "TypeFileInvalidLine";
	case ErrorType::TypeFileTypeOverlapsConstant:
		return "TypeFileTypeOverlapsConstant";
	case ErrorType::TypeFileVariableNameRepeats:
		return "TypeFileVariableNameRepeats";
	case ErrorType::TypeFileVariableNameOverlapsConstant:
		return "TypeFileVariableNameOverlapsConstant";
	case ErrorType::TypeFileVariableNameOverlapsKeyword:
		return "TypeFileVariableNameOverlapsKeyword";
	case ErrorType::TypeFileHasExtraVariable:
		return "TypeFileHasExtraVariable";
	case ErrorType::TypeFileTypeOverlapsKeyword:
		return "TypeFileTypeOverlapsKeyword";
	case ErrorType::TypeFileHasInvalidVariableName:
		return "TypeFileHasInvalidVariableName";
	case ErrorType::TypeFileMissingVariableName:
		return "TypeFileMissingVariableName";
	case ErrorType::OperatorFileInvalidLine:
		return "OperatorFileInvalidLine";
	case ErrorType::OperatorFileMissingOperator:
		return "OperatorFileMissingOperator";
	case ErrorType::OperatorFileHasUndefinedType:
		return "OperatorFileHasUndefinedType";
	case ErrorType::OperatorFileHasUndefinedOperator:
		return "OperatorFileHasUndefinedOperator";
	case ErrorType::OperatorFileMissingCount:
		return "OperatorFileMissingCount";
	case ErrorType::OperatorFileMissingOperatorType:
		return "OperatorFileMissingOperatorType";
	case ErrorType::OperatorFileHasInvalidCount:
		return "OperatorFileHasInvalidCount";
	case ErrorType::OperatorFileMissingValueType:
		return "OperatorFileMissingValueType";
	default:
		throw new std::invalid_argument("Unexpected TypeError element");
	}
}

Error::Error(ErrorType type, std::string description, std::string fileTrace)
{
	m_type = type;
	m_description = description;
	m_fileTrace = fileTrace;
}

std::string Error::GetMessage() const
{
	// Получим сообщение как конкатенацию строк (тип ошибки: описание ошибки)
	std::string message = ErrorTypeToString(m_type) + std::string(": ") + m_description;

	// Если трейс ошибки определен не пустой строкой
	if (m_fileTrace != "")
		// Прибавим к строке трейс ошибки (... | трей ошибки)
		message += std::string(" | ") + m_fileTrace;

	// Вернем сообщение ошибки
	return message;
}

ErrorType Error::GetType() const
{
	// Вернем тип ошибки
	return m_type;
}