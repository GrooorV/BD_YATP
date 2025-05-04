#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"
using namespace std;

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



struct Node
{
	int id; // собственный номер
	Info* dat; // собственная строка
	int tabN; // таблица
	int strN; // элемент   
};



class Table
{
public:
	Table()
	{

	}

	~Table()
	{

	}

	bool addRow(std::string args) {

	}
	//void someMethods() { };

private:

	int num;
	int columnAmount;
	std::string tableName;
	std::string* nameOfColumns; //названия столбцов
	HashMap<std::string, InfoType> columnTypes;

	HashMap<int, Node*> rowIndices;
	DynamicArray<Node*> rows; //массив строк в таблице

};

class Database
{
public:
	Database()
	{

	}

	~Database()
	{

	}

	//void someMethods() { };

private:
	Table* tables; //массив таблиц в бд, динамический массив
};