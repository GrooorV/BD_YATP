#include "BD.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "//124/" << std::endl;
  
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
