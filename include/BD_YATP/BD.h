#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"
#include "Table.h"
#include "Info.h"
#include "Globals.h"

struct Relation {
	int fromTable;
	int toTable;
	std::string fromColumn;
	std::string toColumn;
};


class Database
{
public:

	Database(std::string); // Если не найдёт бд - создаст, если найдёт - загрузит из файла
	~Database();

	void addNewTable();

	bool addTable(std::string);
	
	bool safeAddTable(std::string);

	Table* findTable(int);

	Table* findTable(std::string);

	bool deleteTable(int);

	Node* findById(int);

	bool saveBDInfotoFile(); // сохраняет информацию о базе данных, не сохраняет каждую таблицу по отдельности

	bool saveAllToFiles(); // сохраняет и информацию о базе данных и каждую таблицу по отдельности 

	void printTables();

	void printFullTables();

	bool isLoaded();

private:

	bool isValid;
	std::string name;
	bool loadBDfromFile(std::string);


	DynamicArray<Relation> relations;
	DynamicArray<Table*> tables; //массив таблиц в бд, динамический масси в
	int tableID; // 
};
