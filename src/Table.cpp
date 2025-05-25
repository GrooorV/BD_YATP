#include "Table.h"
#include "Globals.h"
#include <sstream>
#include <fstream>
#include <filesystem>
#include "Validators.h"
#include "BD.h"  
#include "Parsers.h"
#include "PrintUtils.h"


Node::Node(unsigned int d, DynamicArray<Info*> list, int table, bool owns) : id(d), dat(list), tabN(table), own(owns)  {}

Node::~Node()
{
    if (own) {
        for (unsigned int i = 0; i < dat.size(); ++i) {
            delete dat[i];
        }
    }
}

bool Node::isLike(Node* other)
{
    for (int i = 0; i < other->dat.size(); i++) {
        for (int j = 0; j < dat.size(); j++) {
            std::string otherInput = other->dat[i]->getUserInput();
            std::string thisInput = dat[j]->getUserInput();

            if (thisInput.find(otherInput) != std::string::npos || otherInput.find(thisInput) != std::string::npos) {
                return true;
            }
        }
    }
    return false;
}


/* Конструктор создания новой таблицы*/
Table::Table(int number)
{
    std::string token;
    int userType = 0;
    bool usersRightInput = 0;
    num = number;
    curId = ID_START;
    bool seenTable = 0;
    std::cout << "Enter a table name: "  << std::endl;
    while (true)
    {
        std::cout << "AddTable >> ";
        std::getline(std::cin, tableName);
        if (!tableName.empty())
        {
            break;
        std::cout << "its empty, try again" << std::endl;
        }

    }
    std::cout << "How many columns must be in table?" << std::endl;
    while (!usersRightInput)
    {
        std::cout << "AddTable >> ";
        std::getline(std::cin, token);
        if (!isValidInt(token))
            std::cout << "not a valid int number. Try again." << std::endl;
        else
        {
            if (std::stoi(token) <= 0)
                std::cout << "not a valid positive number. Try again." << std::endl;
            else
            {
                columnAmount = std::stoi(token);
                usersRightInput = 1;
            }
        }
    }
    std::cout << "1 - int, 2 - double, 3 - string, 4 - date, 5 - linked, 6 - ManyLinked, 7 - ManyInt." << std::endl;
    nameOfColumns = new std::string[columnAmount];
    columns = new InfoType[columnAmount];
    bool seen = 0;
    for (int i = 0; i < columnAmount; i++)
    {
        while (true)
        {
            seen = 0;
            std::cout << "enter a name of the " << i + 1 << " column" << std::endl;
            std::cout << "AddTable >> ";

            getline(std::cin, nameOfColumns[i]);
            if (nameOfColumns[i].empty())
            {
                std::cout << "its empty, try again" << std::endl;
                continue;
            }
            for (int j = 0; j < i; j++)
            {
                if (nameOfColumns[j] == nameOfColumns[i])
                {
                    std::cout << "Column with that name already exists, try again" << std::endl;
                    seen = 1;
                    break;
                }
            }
            if (!seen)
                break;
        }
        std::cout << "What is type of info in the " << i + 1 << " column?" << std::endl;
        do {
            usersRightInput = 0;
            std::cout << "AddTable >> ";
            std::getline(std::cin, token);
            if (!isValidInt(token))
            {
                usersRightInput = 1;
                std::cout << "not a valid int number. Try again." << std::endl;
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
                    std::cout << "impossible type of info! Try again." << std::endl;
                    usersRightInput = 1;
                    break;
                }

            }


        } while (usersRightInput);
    }
    fileN = "TABLE_" + tableName + ".txt";
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
    return ++curId;
}

std::string Table::getFileName()
{
    return fileN;
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

InfoType Table::getColumnType(const std::string& column) {
    for (int i = 0; i < columnAmount; i++) {
        if (nameOfColumns[i] == column) {
            return columns[i];
        }
    }
    return InfoType::None;
}


/* Успешно ли загрузилась таблица */
bool Table::isLoaded()
{
    return isValid;
}

bool Table::isSimilar(Node* mask, int id, Database* bd)
{
    Node* cur = rowById.find(id);

    if (cur == nullptr) return false;
    if (cur->isLike(mask)) return true;
    if (relations.size() == 0) return false;


    for (int i = 0; i < columnAmount; i++) {
        if (columns[i] == InfoType::Id || columns[i] == InfoType::ManyId) {
            ColumnRelation* r = findRelation(nameOfColumns[i]);
            DynamicArray<int> ar = cur->dat[i]->getIntArray();
            if (bd->findTable(r->toTable)->isSimilar(mask, ar[0], bd)) {
                return true;
            }
        }
    }
    return false;
}




// Методы для работы со строками таблицы 



// Парсит строку от пользователя в вид ID, результат в inp 
bool Table::parseValidIds(std::string& inp, InfoType type, const std::string& columnName, Database* bd) {
    std::string res = "";
    DynamicArray<std::string> tokens = parseStringArray(inp);

    if (tokens.size() > 1 && type == InfoType::Id) {
        return false;
    }
    for (int i = 0; i < tokens.size(); i++) {
        std::string val = tokens[i];


        ColumnRelation* link = findRelation(columnName);
        if (!link) {
            std::cerr << "Invalid relation" << std::endl;
            return false;
        }
        int tablenum = link->toTable;
        Table* cur = bd->findTable(link->toTable);

        if (!cur) {
            std::cerr << "Invalid relation. Cannot find the related table." << std::endl;
            return false;
        }
        int id = 0;
        if (isValidInt(val)) {
            id = cur->getRowId(std::stoi(val) - 1);
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


/* Проверяет распаршенный на части текст на соответствие типу данных, если не подходит - false */
bool Table::isValidPart(const std::string& val, InfoType type) {
    switch (type) {
        case InfoType::Int: return isValidInt(val);
        case InfoType::Double: return isValidDouble(val);
        case InfoType::Date: return isValidDate(val);
        case InfoType::ManyInt: return isValidList(val, '[', ']');
        case InfoType::String: return true;
        default: return false;
    }
}



/* Парсит строку от пользователя в массив result (этот массив сразу в node кладётся*/
bool Table::parseInfo(DynamicArray<Info*>& result, std::string input, Database* bd) {
    int pos = 0, start = 0, tokenCount = 0;

    //сначала разбиваем по пробелам на "токены", проверяем их количество
    DynamicArray<std::string> tokens;

    if (!parseTokens(input, columnAmount, tokens)) return false;


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

    int l = 0;

    for (int i = 0; i < columnAmount; i++) {
        if (columns[i] == InfoType::ManyId || columns[i] == InfoType::Id) {
            l++;
        }
    }
    if (l != relations.size()) {
        std::cout << "Not enough relations set" << std::endl;
        return false;
    }
    DynamicArray<Info*> result;
    if (!parseInfo(result, input, bd)) {
        return false;
    }
    Node* newNode = new Node(genNextId(), result, num);

    rows.append(newNode);
    rowById.insert(newNode->id, newNode);

    return true;
}




/* Редактирует строку, заменяя её на полностью новую */
bool Table::editRow(int nu, std::string input, Database* bd) {
    int id = getRowId(nu);
    if (!findRow(id)) return false;

    DynamicArray<Info*> res;
    if (!parseInfo(res, input, bd)) return false;

    Node* editedNode = new Node(id, res, num);
    for (int i = 0; i < rows.size(); i++) {
        if (rows[i] == rowById.find(id)) {
            delete rows[i];
            rows[i] = editedNode;
        }
    }
    rowById.erase(id);
    rowById.insert(id, editedNode);
    return true;
}

/* Редактирует строку, изменяя только в определённой column */
bool Table::editRowColumn(int nu, std::string column, std::string input, Database* bd) {
    int id = getRowId(nu);

    for (int i = 0; i < columnAmount; i++) {
        if (nameOfColumns[i] == column) {
            if (columns[i] == InfoType::Id || columns[i] == InfoType::ManyId) {
                if (!parseValidIds(input, columns[i], nameOfColumns[i], bd)) return false;
            }
            else {
                std::cout << input << std::endl;
                std::cout << (columns[i] == InfoType::String) << std::endl;
                if (!isValidPart(input, columns[i])) return false;
            }
            std::string s = "";
            for (int j = 0; j < columnAmount; j++) {
                if (nameOfColumns[j] == column) {
                    s += input + " ";
                }
                else {
                    if (rowById.find(id)->dat[j]->getType() == InfoType::String) {
                        s += "\""+ rowById.find(id)->dat[j]->getUserInput() + "\" ";
                    }
                    else {
                        s += rowById.find(id)->dat[j]->getUserInput() + " ";
                    }
                    
                }
            }
            return editRow(nu, s, bd);
            }
        }
    return false;
}

bool Table::deleteRow(int nu, Database* bd) {
    int id = getRowId(nu);
    if (id > rows.size()) return false;
    Node* node = rows[id];
    DynamicArray<Relation*> relati = bd->getRelations();
    for (int i = 0; i < relati.size(); i++) {
        if (relati[i]->toTable == num) {
            Table* cur = bd->findTable(relati[i]->fromTable);
            if (cur->IsInColumn(relati[i]->fromColumn, std::to_string(rows[id]->id))) {
                std::cout << "Can't delete the row. This row has related rows." << std::endl;
                return false;
            }
        }
    }

    rowById.erase(node->id);
    rows.removeValue(node);
    delete node;
    return true;
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
    std::string filename = fileN;
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
    fileN = filename;

    file.close();
    return true;
}

bool Table::deleteFiles() {

    if (!std::filesystem::remove(fileN)) {
        return false;
    }
    std::filesystem::remove(fileN+ ".hash");
    return true;
}




// Методы для красивого вывода

int Table::getColumnWidth(InfoType type, Database* bd, std::string column) {
    switch (type) {
    case InfoType::Int: 
        return INT_COLUMN_WIDTH;
    case InfoType::Id:         
    case InfoType::ManyId: {
        ColumnRelation* link = findRelation(column);
        if (link != nullptr) {
            Table* cur = bd->findTable(link->toTable);
            if (cur) {
                InfoType inf = cur->getColumnType(link->displayColumn);
                return getColumnWidth(inf, bd, column);
            }
        }
        return DEFAULT_COLUMN_WIDTH;
    }
    case InfoType::Double:
        return DOUBLE_COLUMN_WIDTH;
    case InfoType::Date:
        return DATE_COLUMN_WIDTH;
    case InfoType::String:
        return STRING_COLUMN_WIDTH;
    case InfoType::ManyInt: 
        return MANY_COLUMN_WIDTH;
    default:
        return DEFAULT_COLUMN_WIDTH;
    }
}


void Table::printColumnNames(Database* bd) {
    printColumnNamesGen(columns, nameOfColumns, columnAmount,
        [&](InfoType type, const std::string& name) {
            return getColumnWidth(type, bd, name);
        }
    );
}


void Table::PrintRow(Node* row, int number, Database* bd)
{
    for (int i = 0; i < relations.size(); i++) {
        if (!bd->findTable(relations[i]->toTable)) { 
            std::cout << "The related table doesn't exist" << std::endl;
            return; 
        }
    }

    printRowGeneric(
        columns,
        nameOfColumns,
        columnAmount,
        row,
        number,
        [&](const Info* info, int colIdx) {
            return (getRelationText(info->getString(), colIdx, bd));
        },
        [&](InfoType type, const std::string& name) {
            return getColumnWidth(type, bd, name);
        }
    );
}


// Общие методы вывода различные

bool Table::printRow(Node* node, Database* db) {
    if (!node) return false;
    if (!findRow(node->id)) return false;

    PrintRow(node, 1, db);
    return true;
}

void Table::PrintAllRows(Database* db)
{
    for (int i = 0; i < rows.size(); i++) {
        PrintRow(rows[i], i+1, db);
    }
}

void Table::PrintRows(int amount, Database* db)
{
    if (amount < 0) return;
    if (amount > rows.size()) {
        amount = rows.size();
    }
    for (int i = 0; i < amount; i++) {
        PrintRow(rows[i], i+1, db);
    }

}

// Связи

ColumnRelation::ColumnRelation(std::string column, int table, std::string display): columnName(column), toTable(table), displayColumn(display) {}

bool Table::addRelation(std::string fromColumn, int toTable, std::string displayColumn) {
    if (findRelation(fromColumn)) {
        std::cout << "Relation for this column already exists" << std::endl;
        return false;
    }
    if (toTable == num) {
        std::cout << "Cannot relate table to itself" << std::endl;
        return false;
    }
    ColumnRelation* newRelation = new ColumnRelation(fromColumn, toTable, displayColumn);
    relations.append(newRelation);
    return true;

}

bool Table::deleteRelation(std::string fromColumn) {
    if (!findRelation(fromColumn)) {
        std::cout << "Relation for this column doesn't exist" << std::endl;
        return false;
    }
    
    for (int i = 0; i < relations.size(); i++) {
        if (relations[i]->columnName == fromColumn) {
            delete relations[i];
            relations.removeAt(i);
        }
    }
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


std::string Table::findValue(int id, std::string name) {
    for (int i = 0; i < columnAmount; i++) {
        if (nameOfColumns[i] == name) {
            return findRow(id)->dat[i]->getUserInput();
        }
    }
    return "";
}


std::string Table::getRelationText(const std::string& ids, int columnNum, Database* bd) {
    DynamicArray<int> data = parseIntArray(ids);

    ColumnRelation* link = findRelation(nameOfColumns[columnNum]);
    int tablenum = link->toTable;
    Table* cur = bd->findTable(link->toTable);
    if (!cur) return "Unknown connection";
    std::string res = "";
    for (int i = 0; i < data.size(); i++) {
        std::string val = cur->findValue(data[i], link->displayColumn);
        if (i == 0){
            if (val != "") {
                res += val;
            }
            else {
                res += "Unknown connection";
            }
        }
        else {
            if (val != "") {
                res += ", " + val;
            }
            else {
                res += "Unknown connection";
            }
        }
    }
    return res;
}





bool Table::IsInColumn(const std::string& columnName, const std::string& inp) {
    int column = -1;
    for (int i = 0; i < columnAmount; i++) {
        if (columnName == nameOfColumns[i]) {
            column = i;
        }
    }

    if (column == -1) return false;


    for (int i = 0; i < rows.size(); i++) {
        if (rows[i]->dat[column]->getUserInput() == inp) {
            return true;
        }
    }
    return false;

}

DynamicArray<Node*> Table::getSimilar(Node* mask, Database* bd)
{

    DynamicArray<Node*> result;

    for (int i = 0; i < rows.size(); i++) {
        if (rows[i]->isLike(mask)) {
            result.append(rows[i]); 
        }
        else {
            bool flag = false;

            if (relations.size() > 0) {
                for (int j = 0; j < relations.size(); j++) {
                    Table* tmp = bd->findTable(relations[j]->toTable);
                    int typen;
                    for (int k = 0; k < columnAmount; k++) {
                        if (nameOfColumns[k] == relations[j]->columnName) {
                            typen = k;
                        }
                    }
                    DynamicArray<int> tmpAr = rows[i]->dat[typen]->getIntArray();

                    if (tmp->isSimilar(mask, tmpAr[0], bd)) {
                        result.append(rows[i]);
                        break;
                    }

                }
            }
        }
    }
    return result;
}
