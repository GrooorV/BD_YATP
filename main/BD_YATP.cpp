#include "BD.h"
#include <iostream>
#include <string>




int main() {
    std::cout << "//124/" << std::endl;
  
    Database BD = Database("Test");


    
    if (!BD.isLoaded()) std::cout << "PLOHO";

    //�������� ��������
    
    BD.addNewTable();

    Table* table = BD.findTable(1);

    table->addRow("125 1515 661");
    table->addRow("127 1715 761");
    table->addRow("123 1315 161");

    table->PrintAllRows();
    BD.saveAllToFiles();
    

    //�������� ���� ��� ����
    
    /*
    Table* table = BD.findTable(1);

    table->PrintAllRows();
    */

    return 0;
}
