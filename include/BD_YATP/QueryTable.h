#pragma once
#include "BD.h"



class QueryTable {
public:
	QueryTable(Database*, int tableNum, int type, std::string input);
	~QueryTable();

	void filter(int, std::string);

	void print();

	bool isFound();

	int foundRows();

	bool changeShown(std::string);

	void printColumns();
private:

	void collectInfo(Table*, Database*);

	void collectNodeInfo(DynamicArray<Info*>&, Node*, Table*, Database*, int c = 0);

	int columnShownN;
	bool isOk;
	Node* mask;

	DynamicArray<bool> shown;
	DynamicArray<Node*> shownR;
	DynamicArray<Node*> rows;
	DynamicArray<std::string> columnNames;
	DynamicArray<InfoType> columnTypes;
};