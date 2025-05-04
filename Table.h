#pragma once
#include "Info.h"


struct Node
{
	int id; // ����������� �����
	Info* dat; // ����������� ������
	int tabN; // �������
	int strN; // �������   
};



class Table
{
public:
	Table()
	{

	}

	~Table()
	{

	}

	bool addRow(std::string args) {

	}
	//void someMethods() { };

private:

	int num;
	int columnAmount;
	std::string tableName;
	std::string* nameOfColumns; //�������� ��������
	HashMap<std::string, InfoType> columnTypes;

	HashMap<int, Node*> rowIndices;
	DynamicArray<Node*> rows; //������ ����� � �������

};