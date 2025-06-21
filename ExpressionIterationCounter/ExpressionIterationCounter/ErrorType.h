#pragma once
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
	TypeFileTypeOverlapsConstant,
	TypeFileHasExtraVariable,
	TypeFileTypeOverlapsKeyword,
	TypeFileHasInvalidVariableName,
	TypeFileMissingVariableName,
	OperatorFileMissingOperator,
	OperatorFileHasUndefinedType,
	OperatorFileHasUndefinedOperator,
	OperatorFileMissingCount,
	OperatorFileMissingOperatorType,
	OperatorFileHasInvalidCount,
	OperatorFileMissingValueType
};