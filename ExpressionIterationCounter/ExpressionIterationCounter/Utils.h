#pragma once
#include <vector>
#include <string>
#include "Operator.h"

std::vector<std::string> SplitString(std::string str, const char* delimeter);

bool IsOperator(std::string token);

bool IsConstant(std::string str);

bool IsType(std::string str);

bool IsKeyword(std::string str);

bool IsValidVariableName(std::string str);

Operator GetOperatorByToken(std::string token);