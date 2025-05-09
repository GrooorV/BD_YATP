#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"
#include "Table.h"
#include "Info.h"
#include "Globals.h"

class Database
{
public:

	Database(std::string); // Если не найдёт бд - создаст, если найдёт - загрузит из файла
	~Database();

	void addTable();

	Table* findTable(int);

	Node* findById(int);

	bool saveBDInfotoFile(); // сохраняет информацию о базе данных, не сохраняет каждую таблицу по отдельности

	bool saveAllToFiles(); // сохраняет и информацию о базе данных и каждую таблицу по отдельности 

	bool isLoaded();

private:

	bool isValid;
	std::string name;
	bool loadBDfromFile(std::string);

	DynamicArray<Table*> tables; //массив таблиц в бд, динамический масси в
	int tableID; // 
};
