#pragma once
#include <string>
#include "DynamicArray.h"

enum class InfoType {
    Int,
    Double,
    String,
    Date,
    Id,
    ManyInt,
    ManyId,
    None
};

class Info {
private:
    InfoType type;
    std::string userInput;

    union Data {
        int i;
        double d;
        std::string* s;
        DynamicArray<int>* vi;

        Data() {}
        ~Data() {}
    } data;

public:
    Info(InfoType t, const std::string& input);
    ~Info();

    InfoType getType() const;

    int getInt() const;
    double getDouble() const;
    const std::string& getString() const;
    const DynamicArray<int>& getIntArray() const; // геттер для массива

    std::string getUserInput() const;
};
