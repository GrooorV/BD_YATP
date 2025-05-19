#include <sstream>
#include <fstream>
#include "BD.h"
#include "Validators.h"
#include "Checksum.h"

Relation::Relation(int fromTable, int toTable, const std::string& fromColumn, const std::string& toColumn) 
	: fromTable(fromTable),
	toTable(toTable),
	fromColumn(fromColumn),
	toColumn(toColumn)
{}

Database::Database(std::string bname) {
	std::string filename = "BD_" + bname + ".txt";
	isValid = loadBDfromFile(bname);
	if (isValid) return;
	
	name = bname;
	isValid = true;
	tableID = 0;
}


Database::~Database()
{
	for (int i = 0; i < tables.size(); i++) {
		delete tables[i];
	}
}

void Database::addNewTable()
{
	tables.append(new Table(++tableID));
}

Table* Database::findTable(int num)
{
	if (num > tables.size()) return nullptr;

	return tables[num - 1];
}

Table* Database::findTable(std::string name)
{
	for (int i = 0; i < tables.size(); i++) {
		if (tables[i]->getName() == name) {
			return tables[i];
		}
	}
	return nullptr;
}


bool Database::isLoaded()
{
	return isValid;
}

bool Database::saveBDInfotoFile()
{
	std::string filename = "BD_" + name + ".txt";
	std::ofstream file(filename);
	if (!file.is_open()) {
		std::cout << "Unable to open file: " << filename << ". Creating completely new database;" << std::endl;
		return false;
	}

	file << name << '\n' << tableID << '\n';
	file << tables.size() << '\n' << relations.size() << '\n';
	for (int i = 0; i < tables.size(); i++) {
		file << tables[i]->getFileName() << '\n';
	}

	for (int i = 0; i < relations.size(); i++) {
		file << relations[i]->fromColumn << " " << relations[i]->fromTable << " " << relations[i]->toColumn << " " << relations[i]->toTable << '\n';
	}

	file.close();
	return true;
}

bool Database::saveAllToFiles()
{
	for (int i = 0; i < tables.size(); i++) {
		if (!tables[i]->saveToFile()) return false;
	}
	if (!saveBDInfotoFile()) return false;
	return true;
}

bool Database::loadBDfromFile(std::string bname)
{
	std::string filename = "BD_" + bname + ".txt";
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cout << "Unable to open file: " << filename << std::endl;
		return false;
	}
	std::string line;
	if (!std::getline(file, line)) return false;
	name = line;

	std::getline(file, line);
	if (!isValidInt(line)) return false;
	tableID = std::stoi(line);

	int tablesCount = 0;
	int relationsCount = 0;

	if (!std::getline(file, line) || !isValidInt(line)) return false;
	tablesCount = std::stoi(line);

	if (!std::getline(file, line) || !isValidInt(line)) return false;
	relationsCount = std::stoi(line);

	for (int i = 0; i < tablesCount; ++i) {
		if (!std::getline(file, line)) return false;

		Table* table = new Table(line);
		if (!table->isLoaded()) {
			for (int j = 0; j < tables.size(); j++) {
				delete tables[j];
			}
			tables.clear();
			return false;
		}
		tables.append(table);
	}

	for (int i = 0; i < relationsCount; ++i) {
		if (!std::getline(file, line)) return false;
		std::istringstream iss(line);

		int fromTable, toTable;
		std::string fromColumn, toColumn;

		if (!(iss >> fromColumn >> fromTable >> toColumn >> toTable)) {
			std::cout << "Invalid relation format in file.\n";
			return false;
		}
		CreateRelation(fromTable, fromColumn, toTable, toColumn);
	}


	return true;
}


bool Database::safeAddTable(std::string filename) {
	if (!verifyChecksum(filename)) {
		return false;
	}

	Table* newTable = new Table(filename);
	std::string name = newTable->getName();
	for (int i = 0; i < tables.size(); i++)
	{
		if (name == tables[i]->getName())
		{
			std::cout << "Table with name " << name << " already exists" << std::endl;
			return false;
		}
	}
	if (!newTable->isLoaded()) {
		delete newTable;
		return false;
	}
	for (int i = 0; i < tables.size(); i++) {
		if (tables[i]->getId() == newTable->getId()) {
			std::cout << "Table with this ID already exists" << std::endl;
			return false;
		}
	}
	if (tableID < newTable->getId()) {
		tableID = newTable->getId();
	}

	tables.append(newTable);
	return true;

}

bool Database::addTable(std::string filename) {
	Table* newTable = new Table(filename);
	std:: string name = newTable->getName();
	for (int i = 0; i < tables.size(); i++)
	{
		if (name == tables[i]->getName())
		{
			delete newTable;
			std::cout << "Table with name " << name << " already exists" << std::endl;
			return false;
		}
	}
	if (newTable->getId() <= tableID) {
		delete newTable;
		std::cout << "Table with id " << newTable->getId() << " already exists" << std::endl;
		return false;
	}
	if (newTable->getId() > tableID) {
		tableID = newTable->getId() + 1;
	}

	if (!newTable->isLoaded()) {
		delete newTable;
		return false;
	}
	tables.append(newTable);
	return true;
}


bool Database::deleteTable(int id) {
	Table* table = findTable(id);

	if (!table) return false;

	for (int i = 0; i < relations.size(); i++) {
		if (id == relations[i]->fromTable || id == relations[i]->toTable) {
			std::cout << "This table is a part of a relation. Can't delete." << std::endl;
			return false;
		}
	}

	table->deleteFiles();
	tables.removeValue(table);
	delete table;
	saveBDInfotoFile();
	return true;
}

void Database::printTables() {
	for (int i = 0; i < tables.size(); i++) {
		std::cout << i+1 << ". " << tables[i]->getName() << '\n';
	}
}

void Database::printFullTables() {
	for (int i = 0; i < tables.size(); i++) {
		std::cout << tables[i]->getId() << " " << tables[i]->getName() << '\n';
		tables[i]->printColumnNames(this);
		tables[i]->PrintAllRows(this);
		std::cout << std::endl;
	}
}

Node* Database::findById(int id) { // findById(int cellId, int tableId)
	int tableNum = id / ID_AMOUNT;

	Table* table = findTable(tableNum);

	if (!table) return nullptr;

	return table->findRow(id);
	
}

bool Database::CreateRelation(int fromTable, const std::string& fromColumn, int toTable, const std::string& toColumn)
{
	Table* fromtable = findTable(fromTable);
	Table* totable = findTable(toTable);

	if (fromtable == nullptr)
	{
		std::cout << "No such \"fromTable\" " << fromTable << std::endl;
		return false;
	}

	if (totable == nullptr)
	{
		std::cout << "No such \"toTable\" " << toTable << std::endl;
		return false;
	}

	std::string* fromAllNames = fromtable->getNames();
	std::string* toAllNames = totable->getNames();

	int fromAmount = fromtable->getColumnAmount();
	int toAmount = totable->getColumnAmount();
	int first, second;

	for (first = 0; first < fromAmount; first++) //тот ли номер?
	{
		if (fromAllNames[first] == fromColumn) {
			break;
		}
	}

	if (fromAllNames[first] != fromColumn) // верно ли?
	{
		std::cout << "No fromColumn with such name " << fromColumn << std::endl;
		return false;
	}

	for (second = 0; second < toAmount; second++) //тот ли номер?
	{
		if (toAllNames[second] == toColumn)
			break;
	}

	if (second == toAmount) // верно ли?
	{
		std::cout << "No toColumn with such name " << toColumn << std::endl; 
		return false;
	}

	InfoType* colunmsFromTable = fromtable->getInfoTypes();
	InfoType* colunmsToTable = totable->getInfoTypes();

	if ((colunmsFromTable[first] == InfoType::Id || colunmsFromTable[first] == InfoType::ManyId) && (colunmsFromTable[first] != colunmsToTable[second]))
	{
		Relation* link = new Relation(fromTable, toTable, fromColumn, toColumn);
		relations.append(link);

		if (fromtable->addRelation(fromColumn, toTable, toColumn)) {
			return true;
		}
		return false; //убрать
	}
	else {
		std::cout << "this fromColumn " << fromColumn << " from table " << fromTable << " does not contain linked type of data" << std::endl;
		return false;
	}

	return false;
}




DynamicArray<Relation*> Database::getRelations() {
	return relations;
}


int Database::getNumOftables()
{
	return tables.size();
}


bool Database::deleteRelation(int fromTable, const std::string& fromColumn, int toTable, const std::string& toColumn) {
	for (int i = 0; i < relations.size(); i++) {
		if (relations[i]->fromTable == fromTable && relations[i]->fromColumn == fromColumn && relations[i]->toColumn == toColumn && relations[i]->toTable == toTable) {
			Table* cur = findTable(fromTable);
			cur->deleteRelation(fromColumn);
			relations.removeAt(i);
			return true;
		}
	}

	return false;
}