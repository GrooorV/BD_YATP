#include "Validators.h"

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