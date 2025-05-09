#include "Table.h"
#include "Globals.h"
#include <sstream>


bool isValidInt(const std::string& s) {
    if (s.empty()) return false;
    unsigned int i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i == s.size()) return false;
    for (; i < s.size(); ++i)
        if (!isdigit(s[i])) return false;
    return true;
}

bool isValidDouble(const std::string& s) {
    bool dotSeen = false;
    unsigned int i = (s[0] == '-' || s[0] == '+') ? 1 : 0;
    if (i == s.size()) return false;
    for (; i < s.size(); ++i) {
        if (s[i] == '.') {
            if (dotSeen) return false;
            dotSeen = true;
        }
        else if (!isdigit(s[i])) return false;
    }
    return true;
}

bool isValidDate(const std::string& s) {
    if (s.size() != 10) return false;
    return isdigit(s[0]) && isdigit(s[1]) && isdigit(s[2]) && isdigit(s[3]) &&
        s[4] == '-' &&
        isdigit(s[5]) && isdigit(s[6]) &&
        s[7] == '-' &&
        isdigit(s[8]) && isdigit(s[9]);
}

bool isValidList(const std::string& s, char open, char close) {
    if (s.size() < 3 || s.front() != open || s.back() != close) return false;
    std::string content = s.substr(1, s.size() - 2);
    if (content.empty()) return false;

    unsigned int i = 0;
    bool expectingNumber = true;
    while (i < content.size()) {
        unsigned int start = i;
        if (content[i] == '+' || content[i] == '-') i++;
        if (i >= content.size() || !isdigit(content[i])) return false;

        while (i < content.size() && isdigit(content[i])) i++;

        expectingNumber = false;

        if (i == content.size()) break;

        if (content[i] != ',') return false;

        i++;

        if (i == content.size()) return false;
        if (content[i] == ',') return false;

        expectingNumber = true;
    }

    return !expectingNumber;
}

Table::Table(int number)
{
    std::string token;
    int userType = 0;
    bool usersRightInput = 0;
    num = number;
    curId = ID_AMOUNT + num;
    std::cout << "Enter a table name: " << curId << endl;
    std::getline(std::cin, tableName);
    std::cout << "How many columns must be in table?" << endl;
    while (!usersRightInput)
    {
        std::getline(std::cin, token);
        if (!isValidInt(token))
            std::cout << "not a valid int number. Try again." << endl;
        else
        {
            if (std::stoi(token) <= 0)
                std::cout << "not a valid positive number. Try again." << endl;
            else
            {
                columnAmount = std::stoi(token);
                usersRightInput = 1;
            }
        }
    }
    std::cout << "1 - int, 2 - double, 3 - string, 4 - date, 5 - id, 6 - manyId, 7 - ManyInt." << endl;
    nameOfColumns = new std::string[columnAmount];
    columns = new InfoType[columnAmount];
    for (int i = 0; i < columnAmount; i++)
    {
        std::cout << "enter a name of the " << i + 1 << " column" << endl;
        getline( std::cin, nameOfColumns[i] );

        std::cout << "What is type of info in the " << i + 1 << " column?" << endl;
        do {
            usersRightInput = 0;
            std::getline(std::cin, token);
            if (!isValidInt(token))
            {
                usersRightInput = 1;
                std::cout << "not a valid int number. Try again." << endl;
            }
            else
            {
                userType = std::stoi(token);
                switch (userType)
                {
                case 1:
                    columns[i] = InfoType::Int;
                    break;
                case 2:
                    columns[i] = InfoType::Double;
                    break;
                case 3:
                    columns[i] = InfoType::String;
                    break;
                case 4:
                    columns[i] = InfoType::Date;
                    break;
                case 5:
                    columns[i] = InfoType::Id;
                    break;
                case 6:
                    columns[i] = InfoType::ManyId;
                    break;
                case 7:
                    columns[i] = InfoType::ManyInt;
                    break;
                default:
                    std::cout << "impossible type of info! Try again." << endl;
                    usersRightInput = 1;
                    break;
                }

            }


        } while (usersRightInput);
    }
    /*
    columns[0] = InfoType::Int;
    columns[1] = InfoType::Int;
    columns[2] = InfoType::Int;
    columns[3] = InfoType::Int;
    curId = 0;
    num = 0;
    columnAmount = 4;
    */
}

Table::Table()//вызов консруктора для чтения из файла, пока пустой
{

}

unsigned int Table::genNextId()
{
    if ((curId + 1) / ID_AMOUNT != num) {
        std::cout << "OVERFLOW??" << std::endl; // нужно додумать 
        return 0;
    }
    return ++curId;
}

void Table::PrintRow(Node* row)
{
    for (int i = 0; i < columnAmount; i++) {
        std::cout << row->dat[i]->getUserInput() << " ";
    }
    std::cout << std::endl;
}


bool Table::addRow(std::string input)
{
    int pos = 0, start = 0, tokenCount = 0;
    DynamicArray<std::string> tokens;
    for (int i = 0; i < columnAmount; i++) {
        unsigned int next = input.find(' ', pos);
        if (next == std::string::npos && i < columnAmount - 1)
            return false;
        std::string token = input.substr(pos, next - pos);
        tokens.append(token);
        pos = (next == std::string::npos) ? input.size() : next + 1;
    }
    std::cout << input << " HERE 1" << std::endl;
    if (pos < input.size()) return false;

    std::cout << input << " HERE 2" << std::endl;
    DynamicArray<Info*> result;
    
    for (int i = 0; i < columnAmount; ++i) {
        const std::string& val = tokens[i];
        InfoType type = columns[i];

        bool flag = true;
        switch (type) {
        case InfoType::Int:
        case InfoType::Id:
            if (!isValidInt(val)) flag = false;
            break;
        case InfoType::Double:
            if (!isValidDouble(val)) flag = false;
            break;
        case InfoType::Date:
            if (!isValidDate(val)) flag = false;
            break;
        case InfoType::ManyInt:
            if (!isValidList(val, '[', ']')) flag = false;
            break;
        case InfoType::ManyId:
            if (!isValidList(val, '(', ')')) flag = false;
            break;
        case InfoType::String:
            break;
        default:
            return false;
        }

        if (!flag) {
            for (int j = 0; j < i; j++) {
                delete result[j];
            }
            std::cout << input << " HERE :(" << std::endl;
            return false;
        }

        result.append(new Info(type, val));
    }
    std::cout << input << " HERE 3" << std::endl;
    Node* newNode = new Node(genNextId(), result, num);

    rows.append(newNode);
    rowById.insert(newNode->id, newNode);

    return true;
}

void Table::PrintAllRows()
{
    /*
    std::cout << tableName << std::endl;
    for (int i = 0; i < columnAmount; i++)
    {
        std::cout << nameOfColumns[i] << "||";
    }
    std::cout << std::endl;
    */
    for (int i = 0; i < rows.size(); i++) {
        PrintRow(rows[i]);
    }
}

void Table::PrintRows(int amount)
{
    if (amount < 0) return;
    if (amount > rows.size()) {
        amount = rows.size();
    }
    for (int i = 0; i < amount; i++) {
        PrintRow(rows[i]);
    }

}

Node::Node(unsigned int d, DynamicArray<Info*> list, int table): id(d),  dat(list), tabN(table) {}

Node::~Node()
{
    for (unsigned int i = 0; i < dat.size(); ++i) {
        delete dat[i]; 
    }
}
