#include "Info.h"
#include "DynamicArray.h"
#include "Parsers.h"

Info::Info(InfoType t, const std::string& input) : type(t), userInput(input) {
    switch (type) {
        case InfoType::Int:
            data.i = std::stoi(input);
            break;
        case InfoType::Double:
            data.d = std::stod(input);
            break;
        case InfoType::String:
        case InfoType::Date:
            data.s = new std::string(input);
            break;
        case InfoType::ManyId:
        case InfoType::ManyInt:
        case InfoType::Id: {
            data.vi = new DynamicArray<int>(parseIntArray(input));
        }
        case InfoType::None:
            break;
    }
}

Info::~Info() {
    switch (type) {
        case InfoType::String:
        case InfoType::Date:
            delete data.s;
            break;
        case InfoType::ManyInt:
        case InfoType::ManyId:
        case InfoType::Id:
            delete data.vi;
            break;
        default:
            break;
    }
}

InfoType Info::getType() const {
    return type;
}

int Info::getInt() const {
    if (type == InfoType::Int) return data.i;
    if (type == InfoType::Date) {
    int year = std::stoi(data.s->substr(0, 4));
    int month = std::stoi(data.s->substr(5, 2));
    int day = std::stoi(data.s->substr(8, 2));
    return year * 10000 + month * 100 + day;}
}
double Info::getDouble() const {
    if (type == InfoType::Double)return data.d;
}
const std::string& Info::getString() const {
    if (type == InfoType::String || type == InfoType::Date) return *data.s;
    return userInput;
}
const DynamicArray<int>& Info::getIntArray() const {
    if (type == InfoType::ManyInt || type == InfoType::Id) return *data.vi;
}
std::string Info::getUserInput() const {
    return userInput;
}


