#pragma once
#include <iostream>

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
	union {
		// int, double std::string*, date*....
	};

public:
	Info(InfoType t, std::string input) : type(t), userInput(input) {
		switch (t) {
		case InfoType::Int: {
			// обрабатываем ошибки, считаем num, если что-то не так то throw шота
		}

		}
	}

	std::string getInfo();
	double getNum();
	std::string getFileStr();
};

