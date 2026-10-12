#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <stdexcept>

// All money in the project is stored as a whole number of paise
// (Rs. 12.50 is stored as 1250). Integers never have rounding errors,
// unlike float/double, so bills always add up exactly.
typedef long long Money;

const int MAX_INPUT_NUMBER = 1000000000;

// Thrown when the keyboard input ends (Ctrl+Z / Ctrl+D / closed pipe).
// Without it, an input loop would spin forever on a dead stream.
class InputClosedError : public std::runtime_error {
public:
    InputClosedError() : std::runtime_error("Input stream closed.") {}
};

// ---------- formatting and parsing ----------
std::string formatMoney(Money paise);                    // 1250 -> "Rs. 12.50"
bool parseMoney(const std::string& text, Money& out);    // "12.5" -> 1250
bool parseInteger(const std::string& text, long long& out);
bool parseInt(const std::string& text, int& out);

// ---------- text helpers ----------
std::string trim(const std::string& s);
std::string toLower(const std::string& s);
bool containsIgnoreCase(const std::string& text, const std::string& part);
bool isAllDigits(const std::string& s);
std::vector<std::string> splitLine(const std::string& line, char delimiter);
std::string currentDateTime();

// Throws std::invalid_argument if the text is empty or contains '|'
// (the '|' character is our field separator in the data files).
void requireSafeText(const std::string& text, const std::string& fieldName);

// ---------- console input (each one re-asks until the answer is valid) ----------
std::string readText(const std::string& prompt);       // must not be empty
std::string readOptional(const std::string& prompt);   // may be empty
int readInt(const std::string& prompt, int minValue, int maxValue);
Money readMoney(const std::string& prompt);            // positive rupees, e.g. 120 or 120.50
bool readYesNo(const std::string& prompt);

#endif
