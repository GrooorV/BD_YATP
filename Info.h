#pragma once
#include <string>
#include <vector>

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
        //сюды массив
        Data() {}
        ~Data() {}
    } data; //сразу иницилизация

public:
    Info(InfoType t, const std::string& input);
    ~Info();

    InfoType getType() const;

    int getInt() const;
    double getDouble() const;
    const std::string& getString() const;

    std::string getUserInput() const;
};
