#include "ConsoleApp.h"
#include <windows.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <iostream>
#include "HashMap.h"

int main() {

    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
    std::locale::global(std::locale(".UTF-8"));
    std::cin.imbue(std::locale());
    std::cout.imbue(std::locale());


    ConsoleApplication c;
    c.run();

    return 0;
}
