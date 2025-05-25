#include "PrintUtils.h"
#include "Globals.h"


DynamicArray<std::string> wrapText(const std::string& text, int maxWidth) {
    DynamicArray<std::string> lines;
    if (maxWidth <= 0) return lines;

    std::string currentLine;
    int currentWidth = 0;

    for (size_t i = 0; i < text.size(); ) {
        int charLen = 1;
        if ((text[i] & 0xF0) == 0xF0) charLen = 4;
        else if ((text[i] & 0xE0) == 0xE0) charLen = 3;
        else if ((text[i] & 0xC0) == 0xC0) charLen = 2;

        std::string currentChar = text.substr(i, charLen);
        i += charLen;

        if (currentWidth + 1 > maxWidth) {
            lines.append(currentLine);
            currentLine.clear();
            currentWidth = 0;
        }

        currentLine += currentChar;
        currentWidth += 1;
    }

    if (!currentLine.empty()) {
        while (currentWidth < maxWidth) {
            currentLine += ' ';
            currentWidth++;
        }
        lines.append(currentLine);
    }

    return lines;
}



void printDividerGen(InfoType* columns, std::string* nameOfColumns, int columnAmount,
    std::function<int(InfoType type, const std::string& columnName)> getWidth) 
{
    std::cout << "+" << std::string(NUM_COLUMN_WIDTH, '-') << "+";
    for (int i = 0; i < columnAmount; i++) {
        int width = getWidth(columns[i], nameOfColumns[i]);
        std::cout << std::string(width, '-') << "+";
    }
    std::cout << '\n';
}


void printColumnNamesGen(InfoType* columns, std::string* nameOfColumns, int columnAmount, 
    std::function<int(InfoType type, const std::string& columnName)> getWidth) {
    printDividerGen(columns, nameOfColumns, columnAmount, getWidth);
    std::cout << "|" << std::setw(NUM_COLUMN_WIDTH) << std::left << "NUM" << "|";
    for (int i = 0; i < columnAmount; i++) {
        int width = getWidth(columns[i], nameOfColumns[i]);
        if (nameOfColumns[i].length() >= width) {
            std::cout << std::setw(width) << std::left << nameOfColumns[i].substr(0, width - 1) << "|";
        }
        else {
            std::cout << std::setw(width) << std::left << nameOfColumns[i] << "|";
        }
    }
    std::cout << '\n';

    printDividerGen(columns, nameOfColumns, columnAmount, getWidth);
}


void printRowGeneric(InfoType* columns, std::string* nameOfColumns, int columnAmount, Node* row, int number, 
    std::function<std::string(const Info*, int columnIndex)> resolve, 
    std::function<int(InfoType type, const std::string& columnName)> getWidth)
{
    DynamicArray<DynamicArray<std::string>> wrappedColumns;
    int maxHeight = 1;

    for (int i = 0; i < columnAmount; i++) {
        int width = getWidth(columns[i], nameOfColumns[i]);
        std::string inp;
        if ((columns[i] != InfoType::Id && columns[i] != InfoType::ManyId)) {
            if (columns[i] == InfoType::ManyInt) {
                DynamicArray<int> n = row->dat[i]->getIntArray();
                for (int j = 0; j < n.size(); j++) {
                    inp += std::to_string(n[j]) + " ";
                }
            }
            else {
                inp = row->dat[i]->getString();
            }
        }
        else {
            inp = resolve(row->dat[i], i);
        }
        DynamicArray<std::string> lines = wrapText(inp, width);
        if (maxHeight < lines.size()) {
            maxHeight = lines.size();
        }
        wrappedColumns.append(std::move(lines));
    }


    for (int line = 0; line < maxHeight; line++) {
        std::cout << "|";
        if (line == 0) {
            std::cout << std::setw(NUM_COLUMN_WIDTH) << std::left << number;
        }
        else {
            std::cout << std::setw(NUM_COLUMN_WIDTH) << " ";
        }
        std::cout << "|";

        for (int column = 0; column < columnAmount; column++) {
            int width = getWidth(columns[column], nameOfColumns[column]);

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
    printDividerGen(columns, nameOfColumns, columnAmount, getWidth);
}