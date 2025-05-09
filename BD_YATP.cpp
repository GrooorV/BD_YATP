#include "Table.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "///" << std::endl;
  
    Table table(1);

    std::cout << table.addRow("125 616 6166 777 123 ") << std::endl;
    std::cout << table.addRow("125 616 6166 767 123 ") << std::endl;
    std::cout << table.addRow("125 616 6166 747 123 ") << std::endl;
    table.PrintAllRows();
    return 0;
}
