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

			Operation head(Operator::Add);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(10, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
		}

		TEST_METHOD(VariableSameTypes)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Int] = 20;

			Operation head(Operator::Add);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(20, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
		}

		TEST_METHOD(VariableDifferentTypes)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Add] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Add][ValueType::Float] = 22;

			Operation head(Operator::Add);
			head.AddOperand(ValueType::Float);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(22, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
		}

		TEST_METHOD(OneOperandIsOperation)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
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

			Assert::AreEqual(35, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
		}

		TEST_METHOD(BothOperandsAreOperations)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
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

			Assert::AreEqual(42, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
		}

		TEST_METHOD(UnaryOperator)
		{
			std::map<Operator, std::map<ValueType, int>> iterationCorrespondes;
			iterationCorrespondes[Operator::Inversion] = std::map<ValueType, int>();
			iterationCorrespondes[Operator::Inversion][ValueType::Int] = 40;

			Operation head(Operator::Inversion);
			head.AddOperand(ValueType::Int);
			head.AddOperand(ValueType::Int);

			Assert::AreEqual(40, head.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
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

			Assert::AreEqual(105, headOperation.GetIterationsCount(std::set<Operation*>(), iterationCorrespondes));
		}
	};
}
