#include "Table.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "ЧТО ПРОИСХОДЖИТ" << std::endl;
  
    Table table;

    std::cout << table.addRow("125 616 6166 777 ") << std::endl;
    std::cout << table.addRow("125 616 6166 767 ") << std::endl;
    std::cout << table.addRow("125 616 6166 747 ") << std::endl;

    table.PrintAllRows();
    return 0;
}
