#include "QueryTable.h"
#include "Parsers.h"
#include "PrintUtils.h"
#include "Globals.h"

QueryTable::QueryTable(Database* bd, int tableNum, int type, std::string input)
{
	Table* table = bd->findTable(tableNum);
	HashMap<std::string, std::string> result;
	if (type == 1) {
		DynamicArray<std::string> tokens;
		if (!parseTokens(input, 10, tokens)) {
			isOk = false;
			return;
		}
		for (int i = 0; i < tokens.size(); i++) {
			result.insert(tokens[i], tokens[i]);
		}
	}
	else {
		parseConditions(input, result);
	}
	DynamicArray<Info*> infos;
	DynamicArray<std::string> keys =  result.getKeys();


	for (int j = 0; j < keys.size(); j++) {
		infos.append(new Info(InfoType::String, result.find(keys[j])));
	}
	mask = new Node(-1, infos, -1);

	collectInfo(table, bd);

	DynamicArray<Node*> temp = table->getSimilar(mask, bd);

	for (int i = 0; i < temp.size(); i++) {
		DynamicArray<Info*> NEW;
		collectNodeInfo(NEW, temp[i], table, bd);
		rows.append(new Node(-1, NEW, -1));
	}
	
	for (int i = 0; i < columnNames.size(); i++) {
		shown.append(true);
	}
	columnShownN = columnNames.size();

}

QueryTable::~QueryTable()
{
	delete mask;
	for (int i = 0; i < rows.size(); i++) {
		delete rows[i];
	}
}

void QueryTable::filter(int type, std::string input)
{
	HashMap<std::string, std::string> result;
	if (type == 1) {
		DynamicArray<std::string> tokens;
		if (!parseTokens(input, 10, tokens)) {
			isOk = false;
			return;
		}
		for (int i = 0; i < tokens.size(); i++) {
			result.insert(tokens[i], tokens[i]);
		}
	}
	else {
		parseConditions(input, result);
	}
	DynamicArray<Info*> infos;
	DynamicArray<std::string> keys = result.getKeys();


	for (int j = 0; j < keys.size(); j++) {
		infos.append(new Info(InfoType::String, result.find(keys[j])));
	}
	mask = new Node(-1, infos, -1);


	for (int readI = 0; readI < rows.size(); readI++) {
		if (!rows[readI]->isLike(mask)) {
			delete rows[readI];
			rows.removeAt(readI);
			readI--;
		}
	}


}



void QueryTable::print()
{
	std::string* cNames = new std::string[columnShownN];
	InfoType* cTypes = new InfoType[columnShownN];
	int temp = 0;
	for (int i = 0; i < columnNames.size(); i++) {
		if (shown[i]) {
			cNames[temp] = columnNames[i];
			cTypes[temp++] = columnTypes[i];
		}
	}
	auto getWidth = [](InfoType type, const std::string&) {
		switch (type) {
		case InfoType::Int: return INT_COLUMN_WIDTH;
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
		};

	printColumnNamesGen(cTypes, cNames, columnShownN, getWidth);

	for (int i = 0; i < rows.size(); i++) {
		// Create a filtered Info array for the current row
		DynamicArray<Info*> filteredRow;
		int shownIndex = 0;
		for (int j = 0; j < columnNames.size(); j++) {
			if (shown[j]) {
				filteredRow.append(rows[i]->dat[shownIndex]);
				shownIndex++;
			}
		}
		Node* filt = new Node(-1, filteredRow, -1, false);
		printRowGeneric(cTypes, cNames, columnShownN, filt, i + 1,
			[](const Info* info, int) { return info->getString(); },
			getWidth);
		delete filt;
	}
	delete[] cNames;
	delete[] cTypes;

}

bool QueryTable::isFound()
{
	return isOk;
}

int QueryTable::foundRows()
{
	return rows.size();
}

void QueryTable::collectInfo(Table* table, Database* bd)
{
	std::string* tableNames = table->getNames();
	InfoType* tableInfo = table->getInfoTypes();
	int amount = table->getColumnAmount();

	for (int i = 0; i < amount; i++) {
		if (tableInfo[i] != InfoType::Id && tableInfo[i] != InfoType::ManyId) {

			if (tableNames[i] == "Name") {
				columnNames.append(table->getName() + " " + tableNames[i]);
			}
			else {
				columnNames.append(tableNames[i]);
			}
			columnTypes.append(tableInfo[i]);
		}
		else {
			ColumnRelation* curRel = table->findRelation(tableNames[i]);
			collectInfo(bd->findTable(curRel->toTable), bd);
		}
	}
}

void QueryTable::collectNodeInfo(DynamicArray<Info*>& info, Node* node, Table* table, Database* bd, int c)
{

	std::string* tableNames = table->getNames();
	InfoType* tableInfo = table->getInfoTypes();
	int amount = table->getColumnAmount();

	for (int i = c; i < amount; i++) {
		if (tableInfo[i - c] != InfoType::Id && tableInfo[i - c] != InfoType::ManyId) {
			info.append(node->dat[i - c]);
		}
		else {
			ColumnRelation* curRel = table->findRelation(tableNames[i - c]);
			DynamicArray<int> temp = node->dat[i - c]->getIntArray();
			Table* curTable = bd->findTable(curRel->toTable);
			Node* nw = curTable->findRow(temp[0]);
			collectNodeInfo(info, nw, curTable, bd);
		}
	}
}



void QueryTable::printColumns() {
	for (int i = 0; i < columnNames.size(); i++) {
		std::cout << i + 1 << ". " << columnNames[i] << std::endl;
	}
}


bool QueryTable::changeShown(std::string input) {
	for (int i = 0; i < shownR.size(); i++) {
		delete shownR[i];
	}
	shownR.clear();
	DynamicArray<int> ch = parseIntArray(input);

	for (int i = 0; i < columnNames.size(); i++) {
		shown[i] = false;
	}
	columnShownN = 0;

	for (int i = 0; i < ch.size(); i++) {
		if (ch[i] > columnNames.size()) {
			std::cout << "No column has number " << ch[i] << std::endl;
			return false;
		}
		shown[ch[i] - 1] = true;
		columnShownN++;
	}
}