#pragma once
#include <iostream>
#include "BD.h"
#include "DynamicArray.h"
#include <iomanip>
#include <functional>



DynamicArray<std::string> wrapText(const std::string& text, int maxWidth);

void printDividerGen(
    const InfoType*& columns, 
    std::string* nameOfColumns, 
    int columnAmount,
    std::function<int(InfoType type, const std::string& columnName)> getWidth
);


void printColumnNamesGen(
    InfoType* columns, 
    std::string* nameOfColumns, 
    int columnAmount,
    std::function<int(InfoType type, const std::string& columnName)> getWidth
);




void printRowGeneric(
    InfoType* columns,
    std::string* nameOfColumns,
    int columnAmount,
    Node* row,
    int number,
    std::function<std::string(const Info*, int columnIndex)> resolve,
    std::function<int(InfoType type, const std::string& columnName)> getWidth = nullptr
);