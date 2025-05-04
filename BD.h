#pragma once
#include <iostream>
#include "DynamicArray.h"
#include "HashMap.h"
using namespace std;

enum class InfoType {
	Int,
	Double,
	String,
	Date,
	Id,
	ManyInt,
	ManyId,
	None
};


class Info {
private:
	InfoType type;
	std::string userInput;
	union {
		// int, double std::string*, date*....
	};

public:
	Info(InfoType t, std::string input) : type(t), userInput(input) {
		switch (t) {
		case InfoType::Int: {
			// ������������ ������, ������� num, ���� ���-�� �� ��� �� throw ����
		}

		}
	}

	std::string getInfo();
	double getNum();
	std::string getFileStr();
};



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
	Table* tables; //������ ������ � ��, ������������ ������
};