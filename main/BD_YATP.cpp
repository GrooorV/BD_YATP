#include "BD.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "///" << std::endl;
  
    Database BD = Database("Test");


    
    if (!BD.isLoaded()) std::cout << "PLOHO";

    //Проверка создания
    
    BD.addTable();

    Table* table = BD.findTable(1);

    table->addRow("125 1515 661");
    table->addRow("127 1715 761");
    table->addRow("123 1315 161");

    table->PrintAllRows();
    BD.saveAllToFiles();
    

    //Проверка если уже есть
    
    /*
    Table* table = BD.findTable(1);

    table->PrintAllRows();
    */

    return 0;
}
