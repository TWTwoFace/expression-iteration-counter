#pragma once
//! Перечисление, содержащее перечень всех возможных ошибок
enum class ErrorType
{
	FileIsEmpty,
	FileDoesNotExists,
	FileCouldNotBeCreated,
	TreeFileHasExtraLines,
	TreeFileMissingOperator,
	TreeFileHasNotExpression,
	TreeFileHasUndefinedOperators,
	TreeFileMissingOperand,
	TreeFileHasExtraOperands,
	TreeFileFloatingValueAsIndex,
	TypeFileHasUndefinedType,
	TypeFileMissingTypeVariable,
	TypeFileInvalidLine,
	TypeFileTypeOverlapsConstant,
	TypeFileVariableNameRepeats,
	TypeFileVariableNameOverlapsConstant,
	TypeFileVariableNameOverlapsKeyword,
	TypeFileHasExtraVariable,
	TypeFileTypeOverlapsKeyword,
	TypeFileHasInvalidVariableName,
	TypeFileMissingVariableName,
	OperatorFileInvalidLine,
	OperatorFileMissingOperator,
	OperatorFileHasUndefinedType,
	OperatorFileHasUndefinedOperator,
	OperatorFileMissingCount,
	OperatorFileMissingOperatorType,
	OperatorFileHasInvalidCount,
	OperatorFileMissingValueType
};