#include <map>
#include "pch.h"
#include "CppUnitTest.h"
#include "../ExpressionIterationCounter/Operation.h"
#include "../ExpressionIterationCounter/ValueType.h"
#include "../ExpressionIterationCounter/Operator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestExpressionIterationCounter
{
	TEST_CLASS(TestOperation)
	{
	public:
		
		TEST_METHOD(SimpleTest)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;
			iterationCorrespondes[Operator::TakingByIndex] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::TakingByIndex][ValueType::IntArray] = 10;
			iterationCorrespondes[Operator::Or][ValueType::Int] = 10;

			Operation l(Operator::TakingByIndex);
			l.AddOperand(ValueType::Short);
			l.AddOperand(ValueType::IntArray);
			Operation r(Operator::TakingByIndex);
			r.AddOperand(ValueType::Short);
			r.AddOperand(ValueType::IntArray);

			Operation head(Operator::Or);
			head.leftChild = &l;
			head.rightChild = &r;

			Assert::AreEqual(30, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}

		TEST_METHOD(VariableSameTypes)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;

			variableCorrespondes["a"] = ValueType::Int;

			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 20;

			Operation head(Operator::Add);
			head.AddOperand(ValueType::None, "a");
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(20, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}

		TEST_METHOD(VariableDifferentTypes)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;

			variableCorrespondes["a"] = ValueType::Float;

			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Float] = 22;

			Operation head(Operator::Add);
			head.AddOperand(ValueType::None, "a");
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(22, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}

		TEST_METHOD(OneOperandIsOperation)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;

			iterationCorrespondes[Operator::Multiply] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Multiply][ValueType::Int] = 25;

			Operation leftChild(Operator::Multiply);
			leftChild.AddOperand(ValueType::Int);
			leftChild.AddOperand(ValueType::Int);

			Operation head(Operator::Add);
			head.AddOperand(ValueType::Int);
			head.leftChild = &leftChild;

			Assert::AreEqual(35, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}

		TEST_METHOD(BothOperandsAreOperations)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;

			iterationCorrespondes[Operator::Multiply] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Multiply][ValueType::Int] = 20;

			iterationCorrespondes[Operator::Substract] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Substract][ValueType::Int] = 12;

			Operation leftChild(Operator::Multiply);
			leftChild.AddOperand(ValueType::Int);
			leftChild.AddOperand(ValueType::Int);

			Operation rightChild(Operator::Substract);
			rightChild.AddOperand(ValueType::Int);
			rightChild.AddOperand(ValueType::Int);

			Operation head(Operator::Add);
			head.leftChild = &leftChild;
			head.rightChild = &rightChild;

			Assert::AreEqual(42, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}

		TEST_METHOD(UnaryOperator)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;
			iterationCorrespondes[Operator::Inversion] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Inversion][ValueType::Int] = 40;

			Operation head(Operator::Inversion);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(40, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}

		TEST_METHOD(ComplexTest)
		{
			ErrorLogger logger;

			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			std::map<std::string, ValueType> variableCorrespondes;
			iterationCorrespondes[Operator::Multiply] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Multiply][ValueType::Float] = 20;

			iterationCorrespondes[Operator::Not] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Not][ValueType::Int] = 25;

			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;
			iterationCorrespondes[Operator::Add][ValueType::Float] = 20;

			iterationCorrespondes[Operator::Substract] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Substract][ValueType::Float] = 30;

			Operation notOperation(Operator::Not);
			notOperation.AddOperand(ValueType::Int);
			notOperation.AddOperand(ValueType::Int);

			Operation addOperation(Operator::Add);
			addOperation.AddOperand(ValueType::Int);
			addOperation.rightChild = &notOperation;

			Operation multiplyOperation(Operator::Multiply);
			multiplyOperation.AddOperand(ValueType::Float);
			multiplyOperation.AddOperand(ValueType::Int);

			Operation substructOperation(Operator::Substract);
			substructOperation.AddOperand(ValueType::Float);
			substructOperation.rightChild = &addOperation;

			Operation headOperation(Operator::Add);
			headOperation.leftChild = &multiplyOperation;
			headOperation.rightChild = &substructOperation;

			Assert::AreEqual(105, headOperation.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes, variableCorrespondes, logger));
		}
	};
}
