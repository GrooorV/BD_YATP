#include <sstream>
#include <fstream>
#include "BD.h"
#include "Validators.h"
#include "Checksum.h"

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

Table* Database::findTable(int id)
{
	for (int i = 0; i < tables.size(); i++) {
		if (tables[i]->getId() == id) return tables[i];
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

	for (int i = 0; i < tables.size(); i++) {
		file << tables[i]->getFileName() << '\n';
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

	while (std::getline(file, line)) {
		Table* table = new Table(line);
		if (!table->isLoaded()) {
			for (int i = 0; i < tables.size(); i++) {
				delete tables[i];
			}
			return false;
		}
		tables.append(table);
	}

	return true;
}


bool Database::safeAddTable(std::string filename) {
	if (!verifyChecksum(filename)) {
		return false;
	}

	Table* newTable = new Table(filename);
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


	table->deleteFiles();
	tables.removeValue(table);
	delete table;
	saveBDInfotoFile();
	return true;
}

void Database::printTables() {
	for (int i = 0; i < tables.size(); i++) {
		std::cout << tables[i]->getId() << " " << tables[i]->getName() << '\n';
	}
}

void Database::printFullTables() {
	for (int i = 0; i < tables.size(); i++) {
		std::cout << tables[i]->getId() << " " << tables[i]->getName() << '\n';
		tables[i]->printColumnNames();
		tables[i]->PrintAllRows();
		std::cout << std::endl;
	}
}

Node* Database::findById(int id) {
	int tableNum = id / ID_AMOUNT;

	Table* table = findTable(tableNum);

	if (!table) return nullptr;

	return table->findRow(id);
	
}