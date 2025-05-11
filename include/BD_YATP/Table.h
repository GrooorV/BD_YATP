#pragma once
#include "Info.h"
#include "HashMap.h"
#include "DynamicArray.h"

using namespace std;

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
	Table(int number);

	Table(std::string); //вызов консруктора для чтения из файла, пока пустой

	~Table();

	bool addRow(std::string);

	bool deleteRow(int);

	Node* findRow(int);

	DynamicArray<Node*> findInRows(std::string);

	void PrintAllRows();

	void PrintRows(int);

	void printColumnNames();

	std::string getFileName();

	bool saveToFile();

	bool loadFromFile(std::string);
	
	bool sortBy(std::string);

	bool deleteFiles();

	int getId();

	std::string getName();

	bool isLoaded();

	bool printRow(Node*);

private:

	int num;
	int curId;
	int columnAmount;

	bool isValid;
	std::string tableName;
	std::string* nameOfColumns; //названия столбцов
	InfoType* columns; // типы столбцов

	HashMap<int, Node*> rowById;
	DynamicArray<Node*> rows; //массив строк в таблице


	unsigned int genNextId();

	void PrintRow(Node*);
};
