#pragma once
enum class ErrorType
{
	FileDoesNotExists,
	FileCouldNotBeCreated,
	TreeFileHasExtraLines,
	TreeFileHasNotExpression,
	TreeFileHasUndefinedOperators,
	TreeFileMissingOperand,
	TreeFileHasExtraOperands,
	TreeFileFloatingValueAsIndex,
	TreeFileFloatingValuesAsIndex,
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