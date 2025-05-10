#include "BD.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "//124/" << std::endl;
  
    Database BD = Database("Test");


    
    if (!BD.isLoaded()) std::cout << "PLOHO";

    //Проверка создания
    
    BD.addNewTable();

    Table* table = BD.findTable(1);

    table->addRow("125 2025-01-01 AAA");
    table->addRow("127 2023-05-06 BBB");
    table->addRow("123 2025-01-05 CCC");
    
    table->sortBy("1");
    table->PrintAllRows();
    std::cout << std::endl;
    table->sortBy("2");
    table->PrintAllRows();
    std::cout << std::endl;
    table->sortBy("3");
    table->PrintAllRows();
    BD.saveAllToFiles();
    std::cout << std::endl;

    //Проверка если уже есть
    
    /*
    Table* table = BD.findTable(1);

    table->PrintAllRows();
    */

    return 0;
}
