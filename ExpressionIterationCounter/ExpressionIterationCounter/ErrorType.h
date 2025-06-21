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
	OperatorFileMissingOperator,
	OperatorFileHasUndefinedOperator,
	OperatorFileMissingOperatorType
};