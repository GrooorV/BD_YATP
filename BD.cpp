#include <sstream>
#include <fstream>
#include "BD.h"
#include "Validators.h"

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

void Database::addTable()
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
		std::cout << "Unable to open file: " << filename << std::endl;
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



