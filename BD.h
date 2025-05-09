#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"
#include "Table.h"
#include "Info.h"


class Database
{
public:
	Database()
	{
		tableID = 0;
	}

	~Database()
	{

	}

	//void someMethods() { };
	void bootTableManually()//открытие таблицы
	{

	}

	void bootTableFromFile() //пока нет работы с файлами, здесь пусто
	{

	}

private:

	Table* tables; //массив таблиц в бд, динамический масси в
	int tableID; // 
};
