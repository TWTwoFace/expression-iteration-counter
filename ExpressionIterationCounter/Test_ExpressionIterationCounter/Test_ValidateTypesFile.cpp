#include <vector>
#include <string>
#include "pch.h"
#include "CppUnitTest.h"
#include "IsErrorsInLogger.h"
#include "../ExpressionIterationCounter/Validators.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestExpressionIterationCounter
{
	TEST_CLASS(TestValidateTypesFile)
	{
	public:

		TEST_METHOD(UndefinedType)
		{
			std::vector<std::string> fileData = { "string a" };

			std::set<std::string> variables = { "a" };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables, std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileHasUndefinedType }, logger));
		}

		TEST_METHOD(MissingTypeVariable)
		{
			std::vector<std::string> fileData = { };

			std::set<std::string> variables = { "a" };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileMissingTypeVariable }, logger));
		}

		TEST_METHOD(TypeOverlapsConstant)
		{
			std::vector<std::string> fileData = { "true a" };

			std::set<std::string> variables = { };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileTypeOverlapsConstant }, logger));
		}

		TEST_METHOD(HasExtraVariable)
		{
			std::vector<std::string> fileData = { "float a" };

			std::set<std::string> variables = { };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileHasExtraVariable }, logger));
		}

		TEST_METHOD(TypeOverlapsKeyword)
		{
			std::vector<std::string> fileData = { "for a" };

			std::set<std::string> variables = { };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileTypeOverlapsKeyword }, logger));
		}

		TEST_METHOD(CorrectData)
		{
			std::vector<std::string> fileData = { "int a" };

			std::set<std::string> variables = { "a" };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(result && logger.IsEmpty());
		}

		TEST_METHOD(InvalidVariableName)
		{
			std::vector<std::string> fileData = { "int *float" };

			std::set<std::string> variables = { };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileHasInvalidVariableName }, logger));
		}

		TEST_METHOD(MissingVariableName)
		{
			std::vector<std::string> fileData = { "float" };

			std::set<std::string> variables = { };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TypeFileMissingVariableName }, logger));
		}

		TEST_METHOD(ComplexTest)
		{
			std::vector<std::string> fileData = { "for a", "true w", "int" };

			std::set<std::string> variables = { };

			ErrorLogger logger;

			bool result = ValidateTypesFile(fileData, variables,std::map<std::string, bool>(), true, logger);

			Assert::IsTrue(!result && 
				IsErrorsInLogger(
					{ 
						ErrorType::TypeFileTypeOverlapsConstant, 
						ErrorType::TypeFileTypeOverlapsKeyword, 
						ErrorType::TypeFileMissingVariableName
					}, 
					logger));
		}
	};
}
