#include "Error.h"
#include <stdexcept>

constexpr const char* ErrorTypeToString(ErrorType e)
{
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
	case ErrorType::TreeFileHasNotExpression:
		return "TreeFileHasNotExpression";
	case ErrorType::TreeFileHasUndefinedOperators:
		return "TreeFileHasUndefinedOperators";
	case ErrorType::TreeFileMissingOperand:
		return "TreeFileMissingOperand";
	case ErrorType::TreeFileHasExtraOperands:
		return "TreeFileHasExtraOperands";
	case ErrorType::TreeFileFloatingValueAsIndex:
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

std::string Error::GetMessage()
{
	std::string message = ErrorTypeToString(m_type) + std::string(": ") + m_description;

	if (m_fileTrace != "")
		message += std::string(" | ") + m_fileTrace;

	return message;
}

ErrorType Error::GetType()
{
	return m_type;
}