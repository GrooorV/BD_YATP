#pragma once
#include "Info.h"


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