#include "ConsoleApp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

ConsoleApplication::ConsoleApplication()
{
    database = nullptr;
}

void ConsoleApplication::run() {
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        if (command == "exit" || command == "EXIT") return;
        proccessCommand(command);
    }
}

void toLowercase(std::string& input) {
    for (size_t i = 0; i < input.length(); ++i) {
        input[i] = std::tolower(input[i]);
    }
    return;
}


void ConsoleApplication::HELP()
{
    std::cout << ""
        "General Usage:  object [command] [parameters]\n"
        "\n"
        "\n"
        "Description:\n"
        "This is a database editor with basic functions of editing tables \n"
        "EXIT to exit, HELP to help\n"
        "\n"
        "\n"
        "Commands:\n"
        "\n"
        "\n"
        "\n"
        "   EXIT                                              Exits editor\n"
        "\n"
        "\n"
        "\n"
        "   HELP                                              See help\n"
        "\n"
        "\n"
        "\n"
        "   Database commands (BD):\n"
        "\n"
        "   BD CREATE [name]                                  Creates database for editor with name entered. Firstly tries to find database in files, if couldn't find one, creates completely new one to work with\n"
        "\n"
        "\n"
        "   BD ADD                                            Creates new table with constructor. Instruction for creating table in constructor itself\n"
        "\n"
        "\n"
        "   BD ADDFROMFILE [filename]                         Loads table from file with name entered. You can work with this table, checks checksum. If error happened, you still can add table to database, but other errors may happen\n"
        "\n"
        "\n"
        "   BD DELETE [number]                                Deletes table with number entered (info and file itself)\n"
        "\n"
        "\n"
        "   BD SAVE                                           Saves database's info and all its table's info\n"
        "\n"
        "\n"
        "   BD SAVEINFO                                       Saves only database's info\n"
        "\n"
        "\n"
        "   BD PRINT                                          Prints all database's tables with its numbers, names and rows (info)\n"
        "\n"
        "\n"
        "   BD PRINTNAMES                                     Prints only database's tables with numbers and names\n"
        "\n"
        "\n"
        "   BD PRINTROW [id]                                  Prints row from unknown table by its id\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "   Table commands (TABLE), can be booted only if database was loaded or created:\n"
        "\n"
        "   TABLE [number] PRINT                              Prints all headers and rows of table with number entered\n"
        "\n"
        "\n"
        "   TABLE [number] PRINTNAMES                         Prints all headers of table with number entered\n"
        "\n"
        "\n"
        "   TABLE [number] PRINTROWS                           Prints all rows of table with number entered\n"
        "\n"
        "\n"
        "   TABLE [number] PRINTROW [id]                      Prints row with id entered from the table with number entered\n"
        "\n"
        "\n"
        "   TABLE [number] ADD [row]                          Adds new row entered to the table\n"
        "\n"
        "\n"
        "   TABLE [number] ADDMANY                            Boots function to add many rows next to each other\n"
        "\n"
        "\n"
        "   TABLE [number] SORT [name]                        Sorts table by column with name entered. Can sort ints, doubles, strings\n"
        "\n"
        "\n"
        "   TABLE [number] SAVE                               Saves table to file, name takes from name of table\n"
        "\n"
        "\n"
        "   TABLE [number] DELETE [id]                        Deletes row with id entered. To apply changes you need to save file.\n"
        "\n"
        "\n"
        "   Table [number] DELETEMANY                         Boots function to delete many rows next to each other\n"
        "\n"
        "\n"
        "   TABLE [number} FIND [element]                     Prints all rows that contain element entered\n"
        "\n"
        "\n"
        "   TABLE [number] EDIT [id] [row]                    Edits existing row from table with id entered. if elements entered less than in the table, there will remain empty space\n"
        "\n"
        "\n"
        "   TABLE [number] EDITELEMENT [id] [name] [element]  Edits existing element by id of row and mane of column\n"
        "\n"
        "\n"
        "\n"
        "Examples:\n"
        "\n"
        "   BD CREATE\n"
        "\n"
        "   BD ADD\n"
        "\n"
        "   BD ADDFROMFILE TABLE_calculations.txt\n"
        "\n"
        "   TABLE 1 PRINT\n"
        "\n"
        "   TABKE 5 SAVE\n"
        "\n"
        "   TABLE 2 SORT cost\n"
        "\n"
        "   TABLE 4 DELETE 40000015\n"
        "\n"
        "   TABLE 17 EDITELEMENT 170000005 time 22:15\n"
        << std::endl;
    return;
}

void ConsoleApplication::proccessCommand(std::string command)
{
    std::stringstream ss(command);
    std::string action;
    ss >> action;
    toLowercase(action);
    if (action == "help") { HELP();  return; }
    if (action.empty()) return;

    switch (action[0]) {
    case 'b':
        if (action == "bd") {
            proccessBD(ss);
        }
        else {
            std::cout << "Unknown command: " << action << std::endl;
            return;
        }
        break;
    case 't':
        if (action == "table") {
            proccessTable(ss);
        }
        else {
            std::cout << "Unknown command: " << action << std::endl;
            return;
        }
        break;
    default:
        std::cout << "Unknown command: " << action << std::endl;
    }
}

void ConsoleApplication::proccessBD(std::stringstream& ss)
{
    std::string action;
    ss >> action;
    toLowercase(action);
    switch (action[0])
    {


    case 'c':
        if (action == "create")
        {
            std::string name;
            ss >> name;
            if (database == nullptr)
            {
                database = new Database(name);
                std::cout << "Successfully created database " << name << std::endl;
            }
            else {
                std::cout << "Database has been already created. Creating a new one..." << endl;
                delete database;
                database = new Database(name);
                std::cout << "Successfully created database " << name << std::endl;
            }
        }
        else {
            std::cout << "unknown Type of operation with database: " << action << std::endl;
        }
        break;


    case 'a':
        if (action == "add")
        {
            if (database != nullptr)
            {
                database->addNewTable();
                std::cout << "Table was added" << std::endl;
            }
            else {
                std::cout << "Database hasn't been created. Please, create one" << std::endl;
            }
        }
        else
        {
            if (action == "addfromfile")
            {
                if (database != nullptr)
                {
                    std::string filename;
                    ss >> filename;
                    std::ifstream file(filename);
                    if (file.is_open())
                    {
                        if (database->safeAddTable(filename))
                            std::cout << "Table was added from file: " << filename << std::endl;
                        else
                        {
                            std::cout << "Error with Checksum check of file:" << filename << " do you want to boot table anyway, other errors may happen? (y for yes, any other keys for no)" << std::endl;
                            std::string s;
                            getline(std::cin, s);
                            toLowercase(s);
                            if (s == "y")
                            {
                                if (database->addTable(filename))
                                    std::cout << "Table was added from file: " << filename << std::endl;
                                else
                                    std::cout << "Problems with IsValid value, cannot continue" << std::endl;
                            }
                        }
                        file.close();
                    }
                    else {
                        std::cout << "couldn't open file: " << filename << std::endl;
                    }
                }
                else {
                    std::cout << "Database hasn't been created. Please, create one" << std::endl;
                }
            }
            else {
                std::cout << "unknown Type of operation with database: " << action << std::endl;
            }
        }
        break;


    case 'd':
        if (action == "delete")
        {
            if (database != nullptr)
            {
                std::string id;
                ss >> id;
                if (isValidInt(id))
                {
                    if (std::stoi(id) > 0)
                    {
                        if (database->deleteTable(std::stoi(id)))
                            std::cout << "Table " << id << " was successfully deleted" << std::endl;
                        else {
                            std::cout << "Cannot find table with that id" << std::endl;
                        }
                    }
                    else {
                        std::cout << "Id cannot be negaitve" << std::endl;
                    }
                }
                else {
                    std::cout << "This is invalid id, id must be an integer" << std::endl;
                }
            }
            else {
                std::cout << "Database hasn't been created. Please, create one" << std::endl;
            }
        }
        else {
            std::cout << "unknown Type of operation with database: " << action << std::endl;
        }
        break;


    case 's':
        if (action == "save")
        {
            if (database != nullptr)
            {
                if (database->saveAllToFiles())
                    std::cout << "Database was successfully saved" << endl;
                else
                    std::cout << "Couldn't save database to files" << endl;
            }
            else {
                std::cout << "Database hasn't been created. Please, create one" << std::endl;
            }
        }
        else {
            if (action == "saveinfo")
            {
                if (database != nullptr)
                {
                    if (database->saveBDInfotoFile())
                        std::cout << "Database info was successfully saved" << endl;
                    else
                        std::cout << "Couldn't save database info to file" << endl;
                }
                else {
                    std::cout << "Database hasn't been created. Please, create one" << std::endl;
                }
            }
            else {
                std::cout << "unknown Type of operation with database: " << action << std::endl;
            }
        }
        break;



    case 'p':
        if (action == "printnames")
        {
            if (database != nullptr)
            {
                database->printTables();
            }
            else {
                std::cout << "Database hasn't been created. Please, create one" << std::endl;
            }
        }
        else {
            if (action == "printrow")
            {
                if (database != nullptr)
                {
                    std::string rowID;
                    ss >> rowID;
                    if (isValidInt(rowID))
                    {
                        if (std::stoi(rowID) > 0)
                        {
                            Node* node = database->findById(std::stoi(rowID));
                            if (node != nullptr)
                            {
                                Table* table = database->findTable(std::stoi(rowID) / 10000000);
                                table->printColumnNames();
                                table->printRow(node);
                            }
                            else {
                                std::cout << "couldn't find row with this ID: " << rowID << std::endl;
                            }
                        }
                        else {
                            std::cout << "ID must be positive integer" << std::endl;
                        }
                    }
                    else {
                        std::cout << "Not valid ID of table, it must be an integer" << std::endl;
                    }
                } 
                else {
                    std::cout << "Database hasn't been created. Please, create one" << std::endl;
                }
            }
            else {
                if (action == "print")
                {
                    if (database != nullptr)
                    {
                        database->printFullTables();
                    }
                    else {
                        std::cout << "Database hasn't been created. Please, create one" << std::endl;
                    }
                }
                else {
                    std::cout << "unknown Type of operation with database: " << action << std::endl;
                }
            }
        }
        break;


    default:
        std::cout << "unknown Type of operation with database: " << action << std::endl;
    }
}



void ConsoleApplication::proccessTable(std::stringstream& ss)
{
    if (database != nullptr)
    {
        std::string ID;
        ss >> ID;
        if (isValidInt(ID))
        {
            if (std::stoi(ID) > 0)
            {
                int number = stoi(ID);
                Table* table = database->findTable(number);
                if (table != nullptr)
                {
                    std::string action;
                    ss >> action;
                    toLowercase(action);


                    switch (action[0])
                    {


                    case 'p':
                        if (action == "print")
                        {
                            table->printColumnNames();
                            table->PrintAllRows();
                        }
                        else {
                            if (action == "printnames")
                            {
                                table->printColumnNames();
                            }
                            else {
                                if (action == "printallrows")
                                {
                                    table->PrintAllRows();
                                }
                                else {
                                    if (action == "printrow")
                                    {
                                        std::string rowID;
                                        ss >> rowID;
                                        printRowID(table, rowID); //�� ������, ��� ��� ������������ ��� ������
                                    }
                                    else {
                                        std::cout << "Unknown Type of operation with table: " << action << std::endl;
                                    }
                                }
                            }
                        }
                        break;


                    case 'a':
                        if (action == "add")
                        {
                            std::string tmp;
                            std::string toAdd;
                            while (ss >> tmp)
                            {
                                toAdd += tmp + " ";
                            }
                            if (table->addRow(toAdd))
                            {
                                std::cout << "Successfully added new row to table: " << ID << std::endl;
                            }
                            else {
                                std::cout << "Error happend while trying to add row to table: " << ID << ". Possibly there is too much elements or types of info dont match. Try again" << std::endl;
                            }
                        }
                        else {
                            if (action == "addmany")
                            {
                                addMany(table, std::stoi(ID));
                            }
                            else {
                                std::cout << "Unknown Type of operation with table: " << action << std::endl;
                            }
                        }
                        break;


                    case's':
                        if (action == "sort")
                        {
                            std::string sort;
                            ss >> sort;
                            table->printColumnNames();
                            if (table->sortBy(sort))
                            {
                                table->PrintAllRows();
                                std::cout << std::endl;
                            }
                            else {
                                std::cout << "Error while sorting" << std::endl;
                            }
                        }
                        else {
                            if (action == "save")
                            {
                                if (table->saveToFile())
                                {
                                    std::cout << "Successfully saved to file" << std::endl;
                                }
                                else {
                                    std::cout << "Error, couldn't save the table to file" << std::endl;
                                }
                            }
                            else {
                                std::cout << "Unknown Type of operation with table: " << action << std::endl;
                            }
                        }
                        break;


                    case 'd':
                        if (action == "delete")
                        {
                            std::string rowID;
                            ss >> rowID;
                            deleteRowID(table, rowID); //�� ������, ��� ��� ������������ ��� ������
                        }
                        else {
                            if (action == "deletemany")
                            {
                                deleteMany(table, std::stoi(ID));
                            }
                            else {
                                std::cout << "Unknown Type of operation with table: " << action << std::endl;
                            }
                        }
                        break;


                    case 'f':
                        if (action == "find")
                        {
                            std::string find;
                            ss >> find;
                            DynamicArray<Node*> n = table->findInRows(find);
                            table->printColumnNames();
                            if (n.size() > 0)
                            {
                                for (int i = 0; i < n.size(); i++) {
                                    table->printRow(n[i]);
                                }
                                std::cout << "\n";
                            }
                            else {
                                std::cout << "Couldn't find rows with that element: " << find << std::endl;
                            }
                        }
                        else {
                            std::cout << "Unknown Type of operation with table: " << action << std::endl;
                        }
                        break;


                    case 'e':
                        if (action == "edit")
                        {
                            std::string rowID;
                            std::string tmp;
                            std::string toAdd;
                            ss >> rowID;
                            while (ss >> tmp)
                            {
                                toAdd += tmp + " ";
                            }
                            if (isValidInt(rowID))
                            {
                                if (std::stoi(rowID) > 0)
                                {
                                    if (table->editRow(std::stoi(rowID), toAdd))
                                    {
                                        std::cout << "Row was successfully edited" << std::endl;
                                    }
                                    else {
                                        std::cout << "Error, please, check id, amount of elements you are gave on input and info types and try again" << std::endl;
                                    }
                                }
                                else {
                                    std::cout << "ID must be positive integer" << std::endl;
                                }
                            }
                            else {
                                std::cout << "Not valid ID of table, it must be an integer" << std::endl;
                            }
                        }
                        else {
                            if (action == "editelement")
                            {
                                std::string rowID;
                                std::string column;
                                std::string element;
                                ss >> rowID;
                                ss >> column;
                                ss >> element;
                                if (isValidInt(rowID))
                                {
                                    if (std::stoi(rowID) > 0)
                                    {
                                        if (table->editRowColumn(std::stoi(rowID), column, element))
                                        {
                                            std::cout << "Element was successfully edited" << std::endl;
                                        }
                                        else {
                                            std::cout << "Error, please, check id, name of column and info type and try again" << std::endl;
                                        }
                                    }
                                    else {
                                        std::cout << "ID must be positive integer" << std::endl;
                                    }
                                }
                                else {
                                    std::cout << "Not valid ID of table, it must be an integer" << std::endl;
                                }
                            }
                            else {
                                std::cout << "Unknown Type of operation with table: " << action << std::endl;
                            }
                        }
                        break;


                    default:
                        std::cout << "Unknown Type of operation with table: " << action << std::endl;
                    }


                }
                else {
                    std::cout << "There's no table with that number" << std::endl;
                }
            }
            else {
                std::cout << "ID must be positive integer" << std::endl;
            }
        }
        else {
            std::cout << "Not valid ID of table, it must be an integer" << std::endl;
        }
    }
    else {
        std::cout << "Database hasn't been created. Please, create one" << std::endl;
    }
}


void ConsoleApplication::deleteRowID(Table* table, std::string rowID)
{
    if (isValidInt(rowID))
    {
        if (std::stoi(rowID) > 0)
        {
            table->printColumnNames();
            if (table->deleteRow(std::stoi(rowID) - 1))
            {
                table->PrintAllRows();
                std::cout << std::endl;
            }
            else {
                std::cout << "Couldn't find row with that ID" << std::endl;
            }
        }
        else {
            std::cout << "ID must be positive integer" << std::endl;
        }
    }
    else {
        std::cout << "Not valid ID of row, it must be an integer" << std::endl;
    }
}


void ConsoleApplication::printRowID(Table* table, std::string rowID)
{
    if (isValidInt(rowID))
    {
        if (std::stoi(rowID) > 0)
        {
            Node* nuzh = table->findRow(std::stoi(rowID));
            if (nuzh != nullptr)
            {
                table->printColumnNames();
                table->printRow(nuzh);
            }
            else {
                std::cout << "Couldn't find row with this ID: " << rowID << std::endl;
            }
        }
        else {
            std::cout << "ID must be positive integer" << std::endl;
        }
    }
    else {
        std::cout << "Not valid ID of row, it must be an integer" << std::endl;
    }
}

void ConsoleApplication::addMany(Table* table, int ID)
{
    std::string newRow;
    std::cout << "Add new rows by one into table, If you would like to return, type 0" << std::endl;
    while (true)
    {
        std::cout << "add >> ";
        getline(std::cin, newRow);
        if (newRow == "0") return;

        if (table->addRow(newRow))
        {
            table->printColumnNames(); // ����� ����� ����� ������
            table->PrintAllRows(); // ����� ����� ����� ������
            std::cout << "Successfully added new row to table: " << ID << std::endl;
        }
        else {
            std::cout << "Error happend while trying to add row to table: " << ID << ". Possibly there is too much elements or types of info dont match. Try again" << std::endl;
        }
    }
}

void ConsoleApplication::deleteMany(Table* table, int ID)
{
    std::string newID;
    std::cout << "Delete many rows by one in the table, If you would like to return, type 0" << std::endl;
    while (true)
    {
        std::cout << "delete >> ";
        getline(std::cin, newID);
        if (newID == "0") return;

        if (isValidInt(newID))
        {
            if (std::stoi(newID) > 0)
            {
                table->printColumnNames();
                if (table->deleteRow(std::stoi(newID)))
                {
                    table->PrintAllRows();
                    std::cout << std::endl;
                }
                else {
                    std::cout << "Couldn't find row with that ID" << std::endl;
                }
            }
            else {
                std::cout << "ID must be positive integer" << std::endl;
            }
        }
        else {
            std::cout << "Not valid ID of row, it must be an integer" << std::endl;
        }
    }
}
