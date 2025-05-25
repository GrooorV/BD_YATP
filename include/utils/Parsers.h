#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"

DynamicArray<std::string> parseStringArray(const std::string&);

DynamicArray<int> parseIntArray(const std::string&);

bool parseTokens(const std::string& input, int columnAmount, DynamicArray<std::string>& tokens);

bool parseConditions(const std::string&, HashMap<std::string, std::string>&);