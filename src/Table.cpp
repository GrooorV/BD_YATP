#include "Table.h"
#include "Globals.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Validators.h"
#include "BD.h"  



Node::Node(unsigned int d, DynamicArray<Info*> list, int table) : id(d), dat(list), tabN(table) {}

Node::~Node()
{
    for (unsigned int i = 0; i < dat.size(); ++i) {
        delete dat[i];
    }
}




/* Конструктор создания новой таблицы*/
Table::Table(int number)
{
    std::string token;
    int userType = 0;
    bool usersRightInput = 0;
    num = number;
    curId = ID_START;
    //cur
    std::cout << "Enter a table name: " << curId / 100000 << endl;
    std::cout << "AddTable >> ";
    std::getline(std::cin, tableName);
    std::cout << "How many columns must be in table?" << endl;
    while (!usersRightInput)
    {
        std::cout << "AddTable >> ";
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
    std::cout << "1 - int, 2 - double, 3 - string, 4 - date, 5 - linked, 6 - ManyLinked, 7 - ManyInt." << endl;
    nameOfColumns = new std::string[columnAmount];
    columns = new InfoType[columnAmount];
    for (int i = 0; i < columnAmount; i++)
    {
        std::cout << "enter a name of the " << i + 1 << " column" << endl;
        std::cout << "AddTable >> ";
        getline( std::cin, nameOfColumns[i] );

        std::cout << "What is type of info in the " << i + 1 << " column?" << endl;
        do {
            usersRightInput = 0;
            std::cout << "AddTable >> ";
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

/* Конструктор загрузки из файла */
Table::Table(std::string file)
{
    isValid = loadFromFile(file);
}

Table::~Table()
{
    for (int i = 0; i < rows.size(); i++) {
        delete rows[i];
    }
    for (int i = 0; i < relations.size(); i++) {
        delete relations[i];
    }

    rowById.clear();
    delete[] nameOfColumns;
    delete[] columns;
    

}




// Всякие get 
unsigned int Table::genNextId()
{
    if ((curId + 1) / ID_AMOUNT != num) {
        std::cout << "OVERFLOW??" << std::endl; // нужно додумать 
        return ++curId;
    }
    return ++curId;
}

std::string Table::getFileName()
{
    return "TABLE_" + tableName + ".txt";
}

std::string Table::getName() {
    return tableName;
}

int Table::getId()
{
    return num;
}

int Table::getRowId(int num) {
    if (num > rows.size()) {
        return -1;
    }

    return rows[num]->id;
}

/* Успешно ли загрузилась таблица */
bool Table::isLoaded()
{
    return isValid;
}




// Методы для работы со строками таблицы 

/* Проверяет распаршенный на части текст на соответствие типу данных, если не подходит - false */

DynamicArray<std::string> parseStringArray(const std::string& input) {
    DynamicArray<std::string> result;

    int start = input.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return result; // Пустая строка
    }
    int end = input.find_last_not_of(" \t\n\r");
    std::string trimmed = input.substr(start, end - start + 1);

    // Если строка начинается с [ и заканчивается ], это массив
    if (trimmed.size() >= 2 && trimmed[0] == '[' && trimmed.back() == ']') {
        std::string content = trimmed.substr(1, trimmed.size() - 2);
        bool inQuotes = false;
        int startPos = 0;

        for (int i = 0; i < content.size(); ++i) {
            if (content[i] == '"') {
                inQuotes = !inQuotes;
            }
            else if (!inQuotes && content[i] == ',') {
                std::string item = content.substr(startPos, i - startPos);

                int itemStart = item.find_first_not_of(" \t\n\r");
                if (itemStart != std::string::npos) {
                    int itemEnd = item.find_last_not_of(" \t\n\r");
                    item = item.substr(itemStart, itemEnd - itemStart + 1);
                    if (item.size() >= 2 && item[0] == '"' && item.back() == '"') {
                        item = item.substr(1, item.size() - 2);
                    }
                    result.append(item);
                }
                startPos = i + 1;
            }
        }

        if (startPos < content.size()) {
            std::string item = content.substr(startPos);
            int itemStart = item.find_first_not_of(" \t\n\r");
            if (itemStart != std::string::npos) {
                int itemEnd = item.find_last_not_of(" \t\n\r");
                item = item.substr(itemStart, itemEnd - itemStart + 1);
                // Удаляем кавычки, если они есть
                if (item.size() >= 2 && item[0] == '"' && item.back() == '"') {
                    item = item.substr(1, item.size() - 2);
                }
                result.append(item);
            }
        }
    }
    else {
        // Одиночное значение (число или строка)
        if (trimmed.size() >= 2 && trimmed[0] == '"' && trimmed.back() == '"') {
            // Это строка в кавычках
            result.append(trimmed.substr(1, trimmed.size() - 2));
        }
        else {
            // Это число или строка без кавычек
            result.append(trimmed);
        }
    }

    return result;
}


bool Table::parseValidIds(std::string& inp, InfoType type, const std::string& columnName, Database* bd) {
    std::string res = "";
    DynamicArray<std::string> tokens = parseStringArray(inp);

    if (tokens.size() > 1 || type == InfoType::Id) {
        return false;
    }
    for (int i = 0; i < tokens.size(); i++) {
        std::string val = tokens[i];


        ColumnRelation* link = findRelation(columnName);
        int tablenum = link->toTable;
        if (!link) {
            std::cerr << "Invalid relation" << std::endl;
            return false;
        }
        Table* cur = bd->findTable(link->toTable);
        if (!cur) {
            std::cerr << "Invalid relation. Cannot find the related table." << std::endl;
            return false;
        }
        int id = 0;
        if (isValidInt(val)) {
            id = cur->getRowId(std::stoi(val));
            if (id == -1) {
                std::cout << "Table " << link->toTable << " doesn't have row number " << val << std::endl;
                return false;
            }

        }
        else {
            id = cur->findInColumn(link->displayColumn, val);
            if (id == -1) {
                std::cout << "Invalid cell value. Table " << link->toTable << " doesn't contain " << val << " in column " << link->displayColumn << std::endl;
                return false;
            }
        }

        if (i == 0) {
            res += std::to_string(id);
        }
        else {
            res += "," + std::to_string(id);
        }

    }

    inp = res;
    return true;
}



bool Table::isValidPart(const std::string& val, InfoType type) {
    switch (type) {
    case InfoType::Int:
        if (!isValidInt(val)) return false;
        break;
    case InfoType::Double:
        if (!isValidDouble(val)) return false;
        break;
    case InfoType::Date:
        if (!isValidDate(val)) return false;
        break;
    case InfoType::ManyInt:
        if (!isValidList(val, '[', ']')) return false;
        break;
    case InfoType::String:
        return true;
        break;
    default:
        return false;
    }
    return true;
}



/* Парсит строку от пользователя в массив result (этот массив сразу в node кладётся*/
bool Table::parseInfo(DynamicArray<Info*>& result, std::string input, Database* bd) {
    int pos = 0, start = 0, tokenCount = 0;

    //сначала разбиваем по пробелам на "токены", проверяем их количество
    DynamicArray<std::string> tokens;

    while (tokens.size() < columnAmount && pos < input.size()) {
        std::string token;

        if (input[pos] == '"') {
            pos++;
            std::string buffer;

            while (pos < input.size()) {
                char c = input[pos++];

                if (c == '\\') {
                    if (pos >= input.size()) return false;
                    char nextChar = input[pos++];
                    if (nextChar == '"') {
                        buffer += '"';
                    }
                    else {
                        return false;
                    }
                }
                else {
                    if (c == '"') {
                        break;
                    }
                    else {
                        buffer += c;
                    }
                }    
            }
            token = buffer;
            if (pos < input.size() && input[pos] == ' ') {
                pos++;
            }
        }
        else {
            int next = input.find(' ', pos);
            if (next == std::string::npos) {
                token = input.substr(pos);
                pos = input.size();
            }
            else {
                token = input.substr(pos, next - pos);
                pos = next + 1;
            }
        }
        tokens.append(token);
    }

    /*
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
    */
    if (tokens.size() != columnAmount) return false;

    // Проверяем каждый "токен" на соответствие типу данных. Если соответствует, добавляем в массив, иначе возвращаем false
    for (int i = 0; i < columnAmount; ++i) {
        std::string& val = tokens[i];
        InfoType type = columns[i];

        if (type == InfoType::Id || type == InfoType::ManyId) {
            if (!parseValidIds(val, type, nameOfColumns[i], bd)) {
                for (int j = 0; j < i; j++) {
                    delete result[j];
                }
                return false;
            };
        }
        else {
            if (!isValidPart(val, type)) {
                for (int j = 0; j < i; j++) {
                    delete result[j];
                }
                return false;
            }
        }
        result.append(new Info(type, val));
    }
    return true;
}

/* Добавляет НОВУЮ строку в таблицу. Парсит, создаёт ноду */
bool Table::addRow(std::string input, Database* bd)
{
    DynamicArray<Info*> result;
    if (!parseInfo(result, input, bd)) return false;
    Node* newNode = new Node(genNextId(), result, num);

    rows.append(newNode);
    rowById.insert(newNode->id, newNode);

    return true;
}

//////////////////////////////////////////////////////////
/* Редактирует строку, заменяя её на полностью новую */
bool Table::editRow(int id, std::string input) {
    if (!findRow(id)) return false;

    DynamicArray<Info*> res;
    //if (!parseInfo(res, input)) return false;

    Node* editedNode = new Node(id, res, num);
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i] == rowById.find(id)) {
            delete rows[i];
            rows[i] = editedNode;
        }
    }
    rowById.erase(id);
    rowById.insert(id, editedNode);
}

/* Редактирует строку, изменяя только в определённой column */
bool Table::editRowColumn(int id, std::string column, std::string input) {
    for (int i = 0; i < columnAmount; i++) {
        if (nameOfColumns[i] == column) {
            if (isValidPart(input, columns[i])) {
                std::string s = "";
                for (int j = 0; j < columnAmount; j++) {
                    if (nameOfColumns[j] == column) {
                        s += input + " ";
                    }
                    else {
                        s += rowById.find(id)->dat[j]->getUserInput()+ " ";
                    }
                }
                return editRow(id, s);
            }
        }
    }
    return false;
}

bool Table::deleteRow(int id) {

    if (id > rows.size()) return false;
    Node* node = rows[id];

    rowById.erase(node->id);
    rows.removeValue(node);
    delete node;
}

////////////////////////////////////////////////////////////

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

Node* Table::findRow(int id) {
    return rowById.find(id);
}

DynamicArray<Node*> Table::findInRows(std::string subs) {
    DynamicArray<Node*> nodes;

    for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < rows[i]->dat.size(); j++) {
            if (rows[i]->dat[j]->getUserInput().find(subs) != std::string::npos) {
                nodes.append(rows[i]);
                break;
            }
        }
    }

    return nodes;
}


int Table::findInColumn(const std::string& columnName, const std::string& inp) {
    int column = -1;
    for (int i = 0; i < columnAmount; i++) {
        if (columnName == nameOfColumns[i]) {
            column = i;
        }
    }

    if (column == -1) return -1;


    for (int i = 0; i < rows.size(); i++) {
        if (rows[i]->dat[column]->getUserInput() == inp) {
            return rows[i]->id;
        }
    }
    return -1;
}



// Методы работы с файлами

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

bool Table::deleteFiles() {

    if (!std::filesystem::remove(getFileName())) {
        return false;
    }
    std::filesystem::remove(getFileName() + ".hash");
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


void Table::printColumnNames() {
    printDivider(columns, columnAmount);

    std::cout << "|" << std::setw(ID_COLUMN_WIDTH) << std::left << "NUM" << "|";
    for (int i = 0; i < columnAmount; i++) {
        int width = getColumnWidth(columns[i]);
        std::cout << std::setw(width) << std::left << nameOfColumns[i] << "|";
    }
    std::cout << '\n';

    printDivider(columns, columnAmount);
}


void Table::PrintRow(Node* row, int number)
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
            std::cout << std::setw(ID_COLUMN_WIDTH) << std::left << number;
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


// Общие методы вывода различные

bool Table::printRow(Node* node) {
    if (!node) return false;
    if (!findRow(node->id)) return false;

    PrintRow(node, 1);
    return true;
}

void Table::PrintAllRows()
{
    for (int i = 0; i < rows.size(); i++) {
        PrintRow(rows[i], i+1);
    }
}

void Table::PrintRows(int amount)
{
    if (amount < 0) return;
    if (amount > rows.size()) {
        amount = rows.size();
    }
    for (int i = 0; i < amount; i++) {
        PrintRow(rows[i], i+1);
    }

}

// Связи

ColumnRelation::ColumnRelation(std::string column, int table, std::string display): columnName(column), toTable(table), displayColumn(display) {}

void Table::addRelation(std::string fromColumn, int toTable, std::string displayColumn) {
    ColumnRelation* newRelation = new ColumnRelation(fromColumn, toTable, displayColumn);
    relations.append(newRelation);


}

ColumnRelation* Table::findRelation(std::string fromColumn) {
    for (int i = 0; i < relations.size(); i++) {
        if (relations[i]->columnName == fromColumn) {
            return relations[i];
        }
    }
    return nullptr;
}


int Table::getColumnAmount()
{
    return columnAmount;
}

std::string* Table::getNames()
{
    return nameOfColumns;
}

InfoType* Table::getInfoTypes()
{
    return columns;
}