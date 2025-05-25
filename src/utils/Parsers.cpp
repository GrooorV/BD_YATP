#include "Parsers.h"
#include <string>
#include <string_view>


DynamicArray<std::string> parseStringArray(const std::string& input) {
    DynamicArray<std::string> result;

    int start = input.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return result;
    }
    int end = input.find_last_not_of(" \t\n\r");
    std::string trimmed = input.substr(start, end - start + 1);

    // ≈сли строка начинаетс€ с [ и заканчиваетс€ ], это массив
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
                if (item.size() >= 2 && item[0] == '"' && item.back() == '"') {
                    item = item.substr(1, item.size() - 2);
                }
                result.append(item);
            }
        }
    }
    else {
        if (trimmed.size() >= 2 && trimmed[0] == '"' && trimmed.back() == '"') {
            result.append(trimmed.substr(1, trimmed.size() - 2));
        }
        else {
            result.append(trimmed);
        }
    }

    return result;
}


DynamicArray<int> parseIntArray(const std::string& input)
{
    DynamicArray<int> data;
    size_t start = 0;
    size_t end = 0;

    while (start < input.size()) {
        while (start < input.size() &&
            (std::isspace(input[start]) || input[start] == ',' ||
                input[start] == '[' || input[start] == ']')) {
            start++;
        }
        if (start >= input.size()) break;

        end = start;
        while (end < input.size() && (std::isdigit(input[end]) || input[end] == '+' || input[end] == '-')) {
            end++;
        }
        if (end == start) {
            start++;
            continue;
        }

        std::string_view numStr(input.data() + start, end - start);

        int num = 0;
        for (char c : numStr) {
            if (c >= '0' && c <= '9') {
                num = num * 10 + (c - '0');
            }
        }
        data.append(num);

        start = end;
    }
    return data;
}


bool parseTokens(const std::string& input, int columnAmount, DynamicArray<std::string>& tokens) {
    int pos = 0;

    while (tokens.size() < columnAmount && pos < input.size()) {

        while (pos < input.size() && input[pos] == ' ') {
            pos++;
        }
        if (pos >= input.size()) break;

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

    return true;
}


bool parseConditions(const std::string& input, HashMap<std::string, std::string>& result) {

    size_t start = 0;
    size_t end = input.find('"', 1);

    while (start != std::string::npos) {
        while (start < input.size() && std::isspace(input[start])) start++;
        if (start >= input.size()) break;

        if (input[start] != '"') {
            std::cout << "Expected '\"' at position " + std::to_string(start) << std::endl;
        }

        end = input.find('"', start + 1);
        if (end == std::string::npos) {
            std::cout << "Unclosed '\"' for key at position " + std::to_string(start) << std::endl;
            return false;
        }

        std::string key = input.substr(start + 1, end - start - 1);

        start = end + 1;
        while (start < input.size() && std::isspace(input[start])) start++;

        if (start >= input.size() || input[start] != '=') {
            std::cout << "Expected '=' after key at position " + std::to_string(start) << std::endl;
            return false;
        }

        start++;
        while (start < input.size() && std::isspace(input[start])) start++;


        if (start >= input.size() || input[start] != '"') {
            std::cout << "Expected '\"' for value at position " + std::to_string(start) << std::endl;
            return false;
        }


        end = input.find('"', start + 1);
        if (end == std::string::npos) {
            std::cout << "Unclosed '\"' for value at position " + std::to_string(start) << std::endl;
            return false;
        }

        std::string value = input.substr(start + 1, end - start - 1);
        result.insert(key, value); 

        start = input.find('"', end + 1);
    }

    if (result.isEmpty()) {
        std::cout << "No conditions found in query" << std::endl;
    }
    return true;
}