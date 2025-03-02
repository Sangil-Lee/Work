#include <algorithm>
#include <locale>

#include "utilities.h"

namespace s7nodave {

void toLower(std::string& str) {
    for (auto& c : str) {
        c = std::tolower(c, std::locale::classic());
    }
}

void toUpper(std::string& str) {
    for (auto& c : str) {
        c = std::toupper(c, std::locale::classic());
    }
}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::size_t lastPos = 0;
    for (std::size_t pos = 0; pos < str.size(); ++pos) {
        if (str[pos] == delimiter) {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = pos + 1;
        }
    }
    if (lastPos < str.size()) {
        tokens.push_back(str.substr(lastPos, str.size() - lastPos));
    } else {
        tokens.push_back("");
    }
    return tokens;
}

void trim(std::string& str) {
    trimRight(str);
    trimLeft(str);
}

void trimLeft(std::string& str) {
    str.erase(str.begin(), std::find_if_not(str.begin(), str.end(), [](char c) {
        return std::isspace(c, std::locale::classic());
    }));
}

void trimRight(std::string& str) {
    str.erase(std::find_if_not(str.rbegin(), str.rend(), [](char c) {
        return std::isspace(c, std::locale::classic());
    }).base(), str.end());
}

} // namespace s7nodave
