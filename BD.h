#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"
#include "Table.h"
#include "Info.h"
using namespace std;



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