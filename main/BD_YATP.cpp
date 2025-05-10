#include "BD.h"
#include "Validators.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


class ConsoleApplication {
public:
    ConsoleApplication()
    {
        database = nullptr;
    }

    void run() {
        std::string command;
        while (true) {
            std::cout << ">> ";
            std::getline(std::cin, command);

            if (command == "EXIT") break;
           
            if (command == "HELP") HELP();

            processCommand(command);
        }
    }


private:

    Database* database;

    void HELP()
    {
        std::cout << "No help yet" << std::endl;
        return;
    }

    void processCommand(const std::string& command) {
        std::stringstream ss(command);
        std::string action;
        ss >> action;

        if (action.empty()) return;
        if (action.length() == 0) return;
        if (action[0] == '#') return;

        switch (action[0]) {
        case 'B':
            if (action == "BD") {
                proccessBD(ss);
            }
            else {
                std::cout << "Unknown command: " << action << std::endl;
                return;
            }
            break;
        case 'T':
            if (action == "TABLE") {
                //processDoubleDeclaration(ss);
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

    void proccessBD(std::stringstream& ss)
    {
        std::string action;
        ss >> action;
        switch (action[0])
        {


        case 'C':
            if (action == "CREATE")
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


        case 'A':
            if (action == "ADD")
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
                if (action == "ADDFROMFILE")
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


        case 'D':
            if (action == "DELETE")
            {
                if (database != nullptr)
                {
                    std::string id;
                    ss >> id;
                    if (isValidInt(id))
                    {
                        if (std::stoi(id) >= 0)
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


        case 'S':
            if (action == "SAVE")
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
                if (action == "SAVEINFO")
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


        default:
            std::cout << "unknown Type of operation with database: " << action << std::endl;
        }
    }
};


int main() {
    /*
    std::cout << "//124/" << std::endl;
    ConsoleApplication c;
    c.run();
    */





    
    Database BD = Database("Test");


    
    if (!BD.isLoaded()) std::cout << "PLOHO";
    

    //Проверка создания
    
    /*
Tablichka
3
1
1
2
4
3
3
    */
    
    BD.addNewTable();

    Table* table = BD.findTable(1);

    table->addRow("125 2025-01-01 AAA");
    table->addRow("127 2023-05-06 BBBD");
    table->addRow("123 2025-01-05 CCCD");
    

    std::cout << "Test sorting" << std::endl;
    table->sortBy("1");
    table->printColumnNames();
    table->PrintAllRows();
    std::cout << std::endl;


    table->sortBy("2");
    table->PrintAllRows();
    std::cout << std::endl;


    table->sortBy("3");
    table->PrintAllRows();
    std::cout << std::endl;

    std::cout << "Test deleting 10000003" << std::endl;
    table->deleteRow(10000003);
    table->PrintAllRows();
    std::cout << std::endl;

    std::cout << "Test findrows with  D" << std::endl;
    table->addRow("6623 2022-01-05 DDD");
    DynamicArray<Node*> n = table->findInRows("D");
    for (int i = 0; i < n.size(); i++) {
        table->printRow(n[i]);
    }
    std::cout << "\n";

    std::cout << "Test findRow 10000002" << std::endl;
    Node* nuzh = table->findRow(10000002);
    table->printRow(nuzh);


    BD.saveAllToFiles();
    BD.deleteTable(1);
    
    //Проверка если уже есть
    
    /*
    Table* table = BD.findTable(1);

    table->PrintAllRows();
    */

    return 0;
}
