#include "Info.h"

Info::Info(InfoType t, const std::string& input) : type(t), userInput(input) {
    switch (type) {
        case InfoType::Int:
        case InfoType::Id:
            data.i = std::stoi(input);
            break;
        case InfoType::Double:
            data.d = std::stod(input);
            break;
        case InfoType::String:
        case InfoType::Date:
            data.s = new std::string(input);
            break;
        case InfoType::ManyInt:
            // тут пусто d:
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
            //delete data.vi;
            break;
        default:
            break;
    }
}

InfoType Info::getType() const {
    return type;
}

int Info::getInt() const {
    if (type == InfoType::Int || type == InfoType::Id) return data.i;
}
double Info::getDouble() const {
    if (type == InfoType::Double)return data.d;
}
const std::string& Info::getString() const {
    if (type == InfoType::String || type == InfoType::Date) return *data.s;
}
//плейсхолдер возвращения массива
std::string Info::getUserInput() const {
    return userInput;
}


