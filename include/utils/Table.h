#pragma once


#include "Info.h"
#include "HashMap.h"
#include "DynamicArray.h"

class Database;



struct Node
{
	unsigned int id; 
	DynamicArray<Info*> dat; 
	int tabN; 
	bool own;
	Node(unsigned int, DynamicArray<Info*>, int, bool owns=true);

	~Node();

	bool isLike(Node*);
};


struct ColumnRelation {
	std::string columnName;
	int toTable;
	std::string displayColumn;

	ColumnRelation(std::string, int, std::string);
};


class Table
{
public:
	Table(int number); // Конструктор создания новой таблицы
	Table(std::string); // Конструктор чтения из файла
	~Table();

	//Операции со строками
	bool addRow(std::string, Database*);
	bool deleteRow(int, Database*);
	bool editRow(int, std::string, Database*);
	bool editRowColumn(int, std::string, std::string, Database*);


	// Поиск и доступ 
	Node* findRow(int);
	std::string findValue(int, std::string);
	DynamicArray<Node*> findInRows(std::string);
	bool IsInColumn(const std::string& columnName, const std::string& inp);
	DynamicArray<Node*> getSimilar(Node* mask, Database* bd);

	// Вывод
	void printColumnNames(Database*);
	void PrintAllRows(Database*);
	void PrintRows(int, Database*);
	bool printRow(Node*, Database*);


	// Операции с файлами
	std::string getFileName();
	bool saveToFile();
	bool loadFromFile(std::string);
	bool deleteFiles();

	// Get
	int getId();
	std::string getName();
	InfoType getColumnType(const std::string&);
	std::string* getNames();
	int getColumnAmount();
	int getRowId(int);
	InfoType* getInfoTypes();
	bool isLoaded();
	bool isSimilar(Node*, int, Database* bd);


	bool sortBy(std::string);



	//Relations
	bool addRelation(std::string fromColumn, int toTable, std::string displayColumn);
	bool deleteRelation(std::string fromColumn);
	ColumnRelation* findRelation(std::string);

private:

	int num;
	int curId;
	int columnAmount;
	bool isValid;
	std::string tableName;
	std::string fileN;
	std::string* nameOfColumns; //названия столбцов
	InfoType* columns; // типы столбцов


	HashMap<int, Node*> rowById;
	DynamicArray<Node*> rows; //массив строк в таблице
	DynamicArray<ColumnRelation*> relations;


	unsigned int genNextId();

	// Парсинг
	bool parseInfo(DynamicArray<Info*>&, std::string, Database*);
	bool parseValidIds(std::string& inp, InfoType type, const std::string& columnName, Database* bd);


	//Вывод
	void PrintRow(Node*, int, Database*);
	//void printDivider(Database*);
	int getColumnWidth(InfoType type, Database* db, std::string );
	std::string getRelationText(const std::string&, int, Database*);


	// Подтверждалк
	int findInColumn(const std::string& columnName, const std::string& inp);
	bool isValidPart(const std::string&, InfoType);
};
