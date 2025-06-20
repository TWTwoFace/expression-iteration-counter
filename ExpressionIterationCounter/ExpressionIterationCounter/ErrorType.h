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
	TreeFileFloatingValuesAsIndex,
	TypeFileHasUndefinedType,
	TypeFileMissingTypeVariable,
	TypeFileTypeOverlapsConstant,
	OperatorFileMissingOperator,
	OperatorFileHasUndefinedOperator,
	OperatorFileMissingOperatorType
};