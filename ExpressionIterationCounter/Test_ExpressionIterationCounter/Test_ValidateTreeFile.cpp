#include <vector>
#include <string>
#include "pch.h"
#include "CppUnitTest.h"
#include "IsErrorsInLogger.h"
#include "../ExpressionIterationCounter/Validators.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestExpressionIterationCounter
{
	TEST_CLASS(TestValidateTreeFile)
	{
	public:

		TEST_METHOD(MoreThanOneString)
		{
			std::vector<std::string> fileData = { "3 6 +" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TreeFileHasExtraLines }, logger));
		}

		TEST_METHOD(UndefinedOperator)
		{
			std::vector<std::string> fileData = { "3 6 for" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TreeFileHasUndefinedOperators }, logger));
		}

		TEST_METHOD(MissingOperand)
		{
			std::vector<std::string> fileData = { "3 +" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TreeFileMissingOperand }, logger));
		}

		TEST_METHOD(ExtraOperand)
		{
			std::vector<std::string> fileData = { "3 4 8 *" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TreeFileHasExtraOperands }, logger));
		}

		TEST_METHOD(FloatingValueAsIndex)
		{
			std::vector<std::string> fileData = { "a 1.24 []" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TreeFileFloatingValueAsIndex }, logger));
		}

		TEST_METHOD(CorrectValues)
		{
			std::vector<std::string> fileData = { "3 4 + 2 *" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(result && logger.IsEmpty());
		}

		TEST_METHOD(FewErrors)
		{
			std::vector<std::string> fileData = { "a 1.2 [] 23 for" };

			ErrorLogger logger;

			bool result = ValidateTreeFile(fileData, logger);

			Assert::IsTrue(!result && IsErrorsInLogger({ ErrorType::TreeFileFloatingValueAsIndex, ErrorType::TreeFileHasUndefinedOperators }, logger));
		}
	};
}
