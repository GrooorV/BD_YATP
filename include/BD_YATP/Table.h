#pragma once
#include "Info.h"
#include "HashMap.h"
#include "DynamicArray.h"
#include <sstream>

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
	//публичный, что бы можно было записать из бд
	DynamicArray<Table*>* other; // другие таблицы



	Table(int number);

	Table(std::string); //вызов консруктора для чтения из файла, пока пустой

	~Table();

	bool addRow(std::string);

	bool deleteRow(int);

	bool editRow(int, std::string);

	bool editRowColumn(int, std::string, std::string);

	Node* findRow(int);

	DynamicArray<Node*> findInRows(std::string);

	void PrintAllRows();

	void PrintRows(int);

	void printColumnNames();

	// связи
	void initLinks ();

	void editProcess();

	void findAndFill(std::string);

	void updateLinks();

	std::string getFileName();

	bool saveToFile();

	bool loadFromFile(std::string);
	
	bool sortBy(std::string);

	bool deleteFiles();

	int getId();

	Info* getCell(int,int);

	Info* getTarCell(int); 

	std::string getName();

	bool isLoaded();

	bool printRow(Node*);

private:

	int num;
	int curId;
	int columnAmount;
    
	
	DynamicArray<int> links; // "таблица связей"
	int tarId; //таргет на таблицу
	std::string tarCol;


	bool isValid;
	std::string tableName;
	std::string* nameOfColumns; //названия столбцов
	InfoType* columns; // типы столбцов

	HashMap<int, Node*> rowById;
	DynamicArray<Node*> rows; //массив строк в таблице


	unsigned int genNextId();

	void PrintRow(Node*, int);

	bool parseInfo(DynamicArray<Info*>&, std::string);
};
