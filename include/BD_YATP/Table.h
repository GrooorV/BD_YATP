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


struct ColumnRelation {
	std::string columnName;
	int toTable;
	std::string displayColumn;

	ColumnRelation(std::string, int, std::string);
};

class Database;


class Table
{
public:
	Table(int number);

	Table(std::string); //вызов консруктора для чтения из файла, пока пустой

	~Table();

	bool addRow(std::string, Database*);

	bool deleteRow(int);

	bool editRow(int, std::string);

	bool editRowColumn(int, std::string, std::string);

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

	int getRowId(int);

	//Relations

	void addRelation(std::string fromColumn, int toTable, std::string displayColumn);
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

	DynamicArray<ColumnRelation*> relations;

	unsigned int genNextId();

	void PrintRow(Node*, int);

	bool parseInfo(DynamicArray<Info*>&, std::string, Database*);

	int findInColumn(const std::string& columnName, const std::string& inp);

	bool isValidPart(const std::string&, InfoType);

	bool parseValidIds(std::string& inp, InfoType type, const std::string& columnName, Database* bd);

	ColumnRelation* findRelation(std::string);
};
