#include "Table.h"
#include "Globals.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Validators.h"

Table::Table(int number)
{
    std::string token;
    int userType = 0;
    bool usersRightInput = 0;
    num = number;
    curId = ID_AMOUNT * num;
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
    isValid = true;
}

Table::~Table()
{
    for (int i = 0; i < rows.size(); i++) {
        delete rows[i];
    }

    rowById.clear();
    delete[] nameOfColumns;
    delete[] columns;

}

unsigned int Table::genNextId()
{
    if ((curId + 1) / ID_AMOUNT != num) {
        std::cout << "OVERFLOW??" << std::endl; // нужно додумать 
        return 0;
    }
    return ++curId;
}



bool Table::addRow(std::string input)
{
    int pos = 0, start = 0, tokenCount = 0;
    DynamicArray<std::string> tokens;
    for (int i = 0; i < columnAmount; i++) {
        unsigned int next = input.find(' ', pos);
        if (next == std::string::npos) {
            if (i < columnAmount - 1) return false;
            next = input.size();
        }

        std::string token = input.substr(pos, next - pos);
        tokens.append(token);
        pos = next + 1;
    }
    //std::cout << input << " HERE 1" << std::endl;
    if (tokens.size() != columnAmount) return false;


    //std::cout << input << " HERE 2" << std::endl;
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
            //std::cout << input << " HERE :(" << std::endl;
            return false;
        }

        result.append(new Info(type, val));
    }
    //std::cout << input << " HERE 3" << std::endl;
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

std::string Table::getFileName()
{
    return "TABLE_" + tableName + ".txt";
}


InfoType stringToInfoType(const std::string& str) {
    if (str == "Int") return InfoType::Int;
    if (str == "Double") return InfoType::Double;
    if (str == "String") return InfoType::String;
    if (str == "Date") return InfoType::Date;
    if (str == "Id") return InfoType::Id;
    if (str == "ManyInt") return InfoType::ManyInt;
    if (str == "ManyId") return InfoType::ManyId;
    return InfoType::None;
}

std::string infoTypeToString(InfoType type) {
    switch (type) {
    case InfoType::Int: return "Int";
    case InfoType::Double: return "Double";
    case InfoType::String: return "String";
    case InfoType::Date: return "Date";
    case InfoType::Id: return "Id";
    case InfoType::ManyInt: return "ManyInt";
    case InfoType::ManyId: return "ManyId";
    default: return "None";
    }
}


bool Table::saveToFile()
{
    std::string filename = "TABLE_" + tableName + ".txt";
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Unable to open file: " << filename << std::endl;
        return false;
    }

    file << num << ';' << tableName << ';' << columnAmount << ';' << curId << '\n';

    for (int i = 0; i < columnAmount; i++) {
        file << nameOfColumns[i] << ':' << infoTypeToString(columns[i]);
        if (i != columnAmount - 1) {
            file << ';';
        }
    }
    file << '\n';

    for (int i = 0; i < rows.size(); i++) {
        file << rows[i]->id << ';';
        for (int j = 0; j < columnAmount; j++) {
            file << rows[i]->dat[j]->getUserInput() << ';';
        }
        file << '\n';
    }

    file.close();

    return true;
}

bool Table::loadFromFile(std::string filename)
{
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Unable to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    if (!std::getline(file, line)) return false;

    std::istringstream hstream(line);
    std::string token;

    std::getline(hstream, token, ';');

    num = std::stoi(token);

    std::getline(hstream, tableName, ';');

    std::getline(hstream, token, ';');
    columnAmount = std::stoi(token);

    std::getline(hstream, token, ';');
    curId = std::stoi(token);

    nameOfColumns = new std::string[columnAmount];
    columns = new InfoType[columnAmount];

    if (!std::getline(file, line)) return false;
    std::istringstream colStream(line);
    int index = 0;

    while (std::getline(colStream, token, ';') && index < columnAmount) {
        size_t pos = token.find(':');
        if (pos == std::string::npos) continue;

        nameOfColumns[index] = token.substr(0, pos);
        std::string typeStr = token.substr(pos + 1);
        columns[index] = stringToInfoType(typeStr);
        index++;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream rstream(line);
        std::string values[MAX_COLUMNS];
        int valueCount = 0;

        while (std::getline(rstream, token, ';')) {
            values[valueCount++] = token;
        }

        if (valueCount != columnAmount + 1) {
            std::cerr << "Error. Incorrect column amount at " << rows.size() << std::endl;
            continue;
        }

        int rowId = std::stoi(values[0]);

        DynamicArray<Info*> rowData;
        for (int i = 0; i < columnAmount; i++) {
            Info* info = new Info(columns[i], values[i + 1]);
            rowData.append(info);
        }

        Node* node = new Node(rowId, rowData, num);
        rows.append(node);
        rowById.insert(rowId, node);
    }

    file.close();
    return true;
}

int Table::getId()
{
    return num;
}

bool Table::isLoaded()
{
    return isValid;
}

Table::Table(std::string file)
{
    isValid = loadFromFile(file);
}

Node::Node(unsigned int d, DynamicArray<Info*> list, int table): id(d),  dat(list), tabN(table) {}

Node::~Node()
{
    for (unsigned int i = 0; i < dat.size(); ++i) {
        delete dat[i]; 
    }
}


bool Table::sortBy(std::string name) {
    int columnNum = -1;
    
    for (int i = 0; i < columnAmount; i++) {
        if (nameOfColumns[i] == name) {
            columnNum = i;
        }
    }
    if (columnNum == -1) {
        std::cout << "Couldn't find a column with this name" << std::endl;
        return false;
    }


    switch (columns[columnNum]) {
    case InfoType::Int:
    case InfoType::Id: {
        auto compare = [columnNum](Node* a, Node* b) {return a->dat[columnNum]->getInt() < b->dat[columnNum]->getInt(); };
        rows.sort(compare);
        break;
    }
    case InfoType::Double: {
        auto compare = [columnNum](Node* a, Node* b) {return a->dat[columnNum]->getDouble() < b->dat[columnNum]->getDouble(); };
        rows.sort(compare);
        break;
    }
    case InfoType::Date: {
        auto compare = [columnNum](Node* a, Node* b) {return a->dat[columnNum]->getInt() < b->dat[columnNum]->getInt(); };
        rows.sort(compare);
        break;
    }
    case InfoType::String: {
        auto compare = [columnNum](Node* a, Node* b) {return a->dat[columnNum]->getString() < b->dat[columnNum]->getString(); };
        rows.sort(compare);
        break;
    }
    default:
        std::cout << "Unsortable type" << std::endl;
        return false;
    }
    return true;
}


bool Table::deleteFiles() {

    if (!std::filesystem::remove(getFileName())) {
        return false;
    }
    std::filesystem::remove(getFileName() + ".hash");
    return true;
}

bool Table::deleteRow(int id) {
    Node* node = findRow(id);
    if (!node) {
        return false;
    }

    rowById.erase(id);
    rows.removeValue(node);
}

Node* Table::findRow(int id) {
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i]->id == id) {
            return rows[i];
        }
    }
    return nullptr;
}

DynamicArray<Node*> Table::findInRows(std::string subs) {
    DynamicArray<Node*> nodes;

    for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < rows[i]->dat.size(); j++) {
            if (rows[i]->dat[j]->getUserInput().find(subs) != std::string::npos) {
                nodes.append(rows[i]);
            }
        }
    }

    return nodes;
}


bool Table::printRow(Node* node) {
    if (!node) return false;
    if (!findRow(node->id)) return false;

    PrintRow(node);
    return true;
}



// Методы для красивого вывода

int getColumnWidth(InfoType type) {
    switch (type) {
    case InfoType::Int: 
        return INT_COLUMN_WIDTH;
    case InfoType::Id:         
        return ID_COLUMN_WIDTH;
    case InfoType::Double:
        return DOUBLE_COLUMN_WIDTH;
    case InfoType::Date:
        return DATE_COLUMN_WIDTH;
    case InfoType::String:
        return STRING_COLUMN_WIDTH;
    case InfoType::ManyInt: 
    case InfoType::ManyId:
        return MANY_COLUMN_WIDTH;
    default:
        return DEFAULT_COLUMN_WIDTH;
    }
}

DynamicArray<std::string> wrapText(const std::string& text, int width) {
    DynamicArray<std::string> lines;

    for (int i = 0; i < text.size(); i += width) {
        lines.append(text.substr(i, width));
    }
    return lines;
}

void printDivider(InfoType* columns, int amount) {
    std::cout << "+" << std::string(ID_COLUMN_WIDTH, '-') << "+";
    for (int i = 0; i < amount; i++) {
        int width = getColumnWidth(columns[i]);
        std::cout << std::string(width, '-') << "+";
    }
    std::cout << '\n';
}

/*
void printHeaders(std::string* columnNames, InfoType* columnTypes, int amount) {
    printDivider(columnTypes, amount);
    std::cout << "|";
    for (int i = 0; i < amount; i++) {
        int width = getColumnWidth(columnTypes[i]);
        std::cout << std::setw(width) << std::left << columnNames[i] << "|";
    }
    std::cout << '\n';
    printDivider(columnTypes, amount);
}
*/


void Table::printColumnNames() {
    printDivider(columns, columnAmount);

    std::cout << "|" << std::setw(ID_COLUMN_WIDTH) << std::left << "ID" << "|";
    for (int i = 0; i < columnAmount; i++) {
        int width = getColumnWidth(columns[i]);
        std::cout << std::setw(width) << std::left << nameOfColumns[i] << "|";
    }
    std::cout << '\n';

    printDivider(columns, columnAmount);
}


void Table::PrintRow(Node* row)
{
    DynamicArray<DynamicArray<std::string>> wrappedColumns;
    int maxHeight = 1;

    for (int i = 0; i < columnAmount; i++) {
        int width = getColumnWidth(columns[i]);
        DynamicArray<std::string> lines = wrapText(row->dat[i]->getUserInput(), width);

        if (maxHeight < lines.size()) {
            maxHeight = lines.size();
        }
        wrappedColumns.append(std::move(lines));
    }

    for (int line = 0; line < maxHeight; line++) {
        std::cout << "|";
        if (line == 0) {
            std::cout << std::setw(ID_COLUMN_WIDTH) << std::left << row->id;
        }
        else {
            std::cout << std::setw(ID_COLUMN_WIDTH) << " ";
        }
        std::cout << "|";

        for (int column = 0; column < columnAmount; column++) {
            int width = getColumnWidth(columns[column]);

            const auto& lines = wrappedColumns[column];
            if (line < lines.size()) {
                std::cout << std::setw(width) << std::left << lines[line];
            }
            else {
                std::cout << std::setw(width) << " ";
            }
            std::cout << "|";
        }
        std::cout << '\n';
    }
    printDivider(columns, columnAmount);
}