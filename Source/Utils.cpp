#include "Utils.h"

#include <cctype>
#include <ctime>
#include <iostream>

std::string formatMoney(Money paise)
{
    std::string sign = "";
    if (paise < 0) {
        sign = "-";
        paise = -paise;
    }
    long long rupees = paise / 100;
    long long rest = paise % 100;

    std::string result = sign + "Rs. " + std::to_string(rupees) + ".";
    if (rest < 10) {
        result += "0";
    }
    result += std::to_string(rest);
    return result;
}

std::string trim(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        start++;
    }
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        end--;
    }
    return s.substr(start, end - start);
}

std::string toLower(const std::string& s)
{
    std::string r = s;
    for (size_t i = 0; i < r.size(); i++) {
        r[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(r[i])));
    }
    return r;
}

bool containsIgnoreCase(const std::string& text, const std::string& part)
{
    return toLower(text).find(toLower(part)) != std::string::npos;
}

bool isAllDigits(const std::string& s)
{
    if (s.empty()) {
        return false;
    }
    for (size_t i = 0; i < s.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }
    return true;
}

bool parseInteger(const std::string& text, long long& out)
{
    std::string s = trim(text);
    if (!isAllDigits(s) || s.size() > 15) {
        return false;
    }
    out = std::stoll(s);
    return true;
}

bool parseInt(const std::string& text, int& out)
{
    long long value = 0;
    if (!parseInteger(text, value) || value > MAX_INPUT_NUMBER) {
        return false;
    }
    out = static_cast<int>(value);
    return true;
}

bool parseMoney(const std::string& text, Money& out)
{
    std::string s = trim(text);
    size_t dot = s.find('.');
    std::string whole = (dot == std::string::npos) ? s : s.substr(0, dot);
    std::string frac = (dot == std::string::npos) ? "" : s.substr(dot + 1);

    if (!isAllDigits(whole) || whole.size() > 9) {
        return false;
    }
    if (dot != std::string::npos && (frac.empty() || frac.size() > 2 || !isAllDigits(frac))) {
        return false;
    }
    while (frac.size() < 2) {
        frac += "0";
    }
    out = std::stoll(whole) * 100 + std::stoll(frac);
    return true;
}

std::vector<std::string> splitLine(const std::string& line, char delimiter)
{
    std::vector<std::string> parts;
    std::string current = "";
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i] == delimiter) {
            parts.push_back(current);
            current = "";
        } else {
            current += line[i];
        }
    }
    parts.push_back(current);
    return parts;
}

std::string currentDateTime()
{
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", local);
    return std::string(buffer);
}

void requireSafeText(const std::string& text, const std::string& fieldName)
{
    if (text.empty()) {
        throw std::invalid_argument(fieldName + " cannot be empty.");
    }
    if (text.find('|') != std::string::npos) {
        throw std::invalid_argument(fieldName + " cannot contain the '|' character.");
    }
}

// ---------------------------------------------------------------------------
// Console input
// ---------------------------------------------------------------------------

static std::string getLineOrThrow(const std::string& prompt)
{
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw InputClosedError();
    }
    return trim(line);
}

std::string readOptional(const std::string& prompt)
{
    return getLineOrThrow(prompt);
}

std::string readText(const std::string& prompt)
{
    while (true) {
        std::string s = getLineOrThrow(prompt);
        if (!s.empty()) {
            return s;
        }
        std::cout << "  Input cannot be empty.\n";
    }
}

int readInt(const std::string& prompt, int minValue, int maxValue)
{
    while (true) {
        std::string s = getLineOrThrow(prompt);
        int value = 0;
        if (parseInt(s, value) && value >= minValue && value <= maxValue) {
            return value;
        }
        std::cout << "  Please enter a whole number from " << minValue
                  << " to " << maxValue << ".\n";
    }
}

Money readMoney(const std::string& prompt)
{
    while (true) {
        std::string s = getLineOrThrow(prompt);
        Money value = 0;
        if (parseMoney(s, value) && value > 0) {
            return value;
        }
        std::cout << "  Enter a positive amount such as 120 or 120.50.\n";
    }
}

bool readYesNo(const std::string& prompt)
{
    while (true) {
        std::string s = toLower(getLineOrThrow(prompt));
        if (s == "y" || s == "yes") {
            return true;
        }
        if (s == "n" || s == "no") {
            return false;
        }
        std::cout << "  Please answer y or n.\n";
    }
}
