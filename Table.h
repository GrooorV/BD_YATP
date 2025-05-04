#pragma once
#include "Info.h"
#include "HashMap.h"
#include "DynamicArray.h"

struct Node
{
	unsigned int id; // собственный номер
	DynamicArray<Info*> dat; // собственная строка
	int tabN; // таблица

	Node(unsigned int, DynamicArray<Info*>, int);

	~Node();
};



class Table
{
public:
	Table()
	{
		// Только для теста
		columns = new InfoType[4];
		columns[0] = InfoType::Int;
		columns[1] = InfoType::Int;
		columns[2] = InfoType::Int;
		columns[3] = InfoType::Int;
		curId = 0;
		num = 0;
		columnAmount = 4;
	}

	~Table()
	{

	}

	bool addRow(std::string);

	void PrintAllRows();

	void PrintRows(int);
	//void someMethods() { };

private:

	int num;
	int curId;
	int columnAmount;
	std::string tableName;
	std::string* nameOfColumns; //названия столбцов
	InfoType* columns; // типы столбцов

	HashMap<int, Node*> rowById;
	DynamicArray<Node*> rows; //массив строк в таблице


	unsigned int genNextId();

	void PrintRow(Node*);
};
