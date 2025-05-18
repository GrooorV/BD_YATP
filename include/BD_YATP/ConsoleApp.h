#pragma once
#include "BD.h"
#include "Validators.h"


void toLowercase(std::string& input);

class ConsoleApplication {
public:

	ConsoleApplication();
	void run();

private:

	Database* database;

	void HELP();

	void proccessBD(std::stringstream& ss);

	void proccessTable(std::stringstream& ss);

	void deleteRowID(Table* table, std::string rowID);

	void printRowID(Table* table, std::string rowID);

	void addMany(Table* table, int ID);

	void deleteMany(Table* table, int ID);
};
