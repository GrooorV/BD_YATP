#include "Info.h"
#include "DynamicArray.h"

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
        case InfoType::ManyId:
        case InfoType::ManyInt: {
            data.vi = new DynamicArray<int>();
            size_t start = 0;
            size_t end = 0;
            // фан факт без трайев не будет работать
            // логика проста, через запятые вводите и все на базе йоу
            // "1,2,3,12,13"
            while ((end = input.find(',', start)) != std::string::npos) {
                std::string token = input.substr(start, end - start);
                try {
                    int value = std::stoi(token);
                    data.vi->append(value);
                } catch (...){}
                start = end + 1;
            }

            if (start < input.length()) {
                try {
                    int value = std::stoi(input.substr(start));
                    data.vi->append(value);
                } catch (...) {}
            }
            break;
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
    if (type == InfoType::Int || type == InfoType::Id) return data.i;
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
}
const DynamicArray<int>& Info::getIntArray() const {
    if (type == InfoType::ManyInt) return *data.vi;
}
std::string Info::getUserInput() const {
    return userInput;
}


