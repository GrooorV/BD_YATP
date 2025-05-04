#include "Table.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "ЧТО ПРОИСХОДЖИТ" << std::endl;
  
    Table table;

    table.addRow("125 616 6166 777");
    table.addRow("125 616 6166 777");
    table.addRow("125 616 6166 777");

    table.PrintAllRows();
    return 0;
}
