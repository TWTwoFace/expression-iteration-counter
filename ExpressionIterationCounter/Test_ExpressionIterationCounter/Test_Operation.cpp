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
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;

			Operation head(Operator::Add, &iterationCorrespondes);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(10, head.GetIterationsCount(std::set<Operation*>()));
		}

		TEST_METHOD(VariableSameTypes)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 20;

			Operation head(Operator::Add, &iterationCorrespondes);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(20, head.GetIterationsCount(std::set<Operation*>()));
		}

		TEST_METHOD(VariableDifferentTypes)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Float] = 22;

			Operation head(Operator::Add, &iterationCorrespondes);
			head.AddOperand(ValueType::Float);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(22, head.GetIterationsCount(std::set<Operation*>()));
		}

		TEST_METHOD(OneOperandIsOperation)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;

			iterationCorrespondes[Operator::Multiply] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Multiply][ValueType::Int] = 25;

			Operation leftChild(Operator::Multiply, &iterationCorrespondes);
			leftChild.AddOperand(ValueType::Int);
			leftChild.AddOperand(ValueType::Int);

			Operation head(Operator::Add, &iterationCorrespondes);
			head.AddOperand(ValueType::Int);
			head.leftChild = &leftChild;

			Assert::AreEqual(35, head.GetIterationsCount(std::set<Operation*>()));
		}

		TEST_METHOD(BothOperandsAreOperations)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;

			iterationCorrespondes[Operator::Multiply] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Multiply][ValueType::Int] = 20;

			iterationCorrespondes[Operator::Substract] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Substract][ValueType::Int] = 10;

			Operation leftChild(Operator::Multiply, &iterationCorrespondes);
			leftChild.AddOperand(ValueType::Int);
			leftChild.AddOperand(ValueType::Int);

			Operation rightChild(Operator::Substract, &iterationCorrespondes);
			rightChild.AddOperand(ValueType::Int);
			rightChild.AddOperand(ValueType::Int);

			Operation head(Operator::Add, &iterationCorrespondes);
			head.leftChild = &leftChild;
			head.rightChild = &rightChild;

			Assert::AreEqual(42, head.GetIterationsCount(std::set<Operation*>()));
		}

		TEST_METHOD(UnaryOperator)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Inversion] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Inversion][ValueType::Int] = 40;

			Operation head(Operator::Inversion, &iterationCorrespondes);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(40, head.GetIterationsCount(std::set<Operation*>()));
		}

		TEST_METHOD(ComplexTest)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Multiply] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Multiply][ValueType::Float] = 20;

			iterationCorrespondes[Operator::Not] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Not][ValueType::Int] = 25;

			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 10;
			iterationCorrespondes[Operator::Add][ValueType::Float] = 20;

			iterationCorrespondes[Operator::Substract] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Substract][ValueType::Float] = 30;

			Operation notOperation(Operator::Not, &iterationCorrespondes);
			notOperation.AddOperand(ValueType::Int);
			notOperation.AddOperand(ValueType::Int);

			Operation addOperation(Operator::Add, &iterationCorrespondes);
			addOperation.AddOperand(ValueType::Int);
			addOperation.rightChild = &notOperation;

			Operation multiplyOperation(Operator::Multiply, &iterationCorrespondes);
			multiplyOperation.AddOperand(ValueType::Float);
			multiplyOperation.AddOperand(ValueType::Int);

			Operation substructOperation(Operator::Substract, &iterationCorrespondes);
			substructOperation.AddOperand(ValueType::Float);
			substructOperation.rightChild = &addOperation;

			Operation headOperation(Operator::Add, &iterationCorrespondes);
			headOperation.leftChild = &multiplyOperation;
			headOperation.rightChild = &substructOperation;

			Assert::AreEqual(105, headOperation.GetIterationsCount(std::set<Operation*>()));
		}
	};
}
