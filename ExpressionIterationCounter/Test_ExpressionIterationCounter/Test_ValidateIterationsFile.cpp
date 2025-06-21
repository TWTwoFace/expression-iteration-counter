#include <vector>
#include <string>
#include "pch.h"
#include "CppUnitTest.h"
#include "IsErrorsInLogger.h"
#include "../ExpressionIterationCounter/Validators.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestExpressionIterationCounter
{
	TEST_CLASS(TestValidateIterationsFile)
	{
	public:

		TEST_METHOD(UndefinedOperator)
		{
			std::vector<std::string> fileData = { "+ int 1", "for int 15" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileHasUndefinedOperator }, logger));
		}

		TEST_METHOD(UndefinedType)
		{
			std::vector<std::string> fileData = { "+ string 10" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileHasUndefinedType }, logger));
		}

		TEST_METHOD(MissingCount)
		{
			std::vector<std::string> fileData = { "+ int" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileMissingCount }, logger));
		}

		TEST_METHOD(FloatedCount)
		{
			std::vector<std::string> fileData = { "+ int 1.2" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileHasInvalidCount }, logger));
		}

		TEST_METHOD(NegativeCount)
		{
			std::vector<std::string> fileData = { "+ int -1" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileHasInvalidCount }, logger));
		}

		TEST_METHOD(LiteralCount)
		{
			std::vector<std::string> fileData = { "+ int abc" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileHasInvalidCount }, logger));
		}

		TEST_METHOD(MissingValueType)
		{
			std::vector<std::string> fileData = { "+ 1" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileMissingValueType }, logger));
		}

		TEST_METHOD(MissingOperator)
		{
			std::vector<std::string> fileData = { };

			std::set<Operator> operators = { Operator::Add };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::OperatorFileMissingOperator }, logger));
		}

		TEST_METHOD(ComplexTest)
		{
			std::vector<std::string> fileData = { "+ int", "- float a", "* abc 2"};

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(!result && 
				IsErrorsInLogger(
					{ 
						ErrorType::OperatorFileMissingCount, 
						ErrorType::OperatorFileHasInvalidCount, 
						ErrorType::OperatorFileHasUndefinedType 
					},
					logger));
		}

		TEST_METHOD(CorrectData)
		{
			std::vector<std::string> fileData = { "+ int 12", "- float 10", "~ int 22" };

			std::set<Operator> operators = { };

			ErrorLogger logger;

			bool result = ValidateIterationsFile(fileData, operators, logger);

			Assert::IsTrue(result && logger.IsEmpty());
		}
	};
}
