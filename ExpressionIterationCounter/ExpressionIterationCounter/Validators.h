#pragma once
#include <vector>
#include <string>
#include <set>
#include "Logger.h"
#include "Operator.h"

bool ValidateTreeFile(const std::vector<std::string>& fileData, ErrorLogger& logger);

bool ValidateTypesFile(const std::vector<std::string>& fileData, const std::set<std::string>& variables, ErrorLogger& logger);

bool ValidateIterationsFile(const std::vector<std::string>& fileData, const std::set<Operator>& operators, ErrorLogger& logger);