#include <string>
#include <vector>

#ifndef s7nodaveUtilities_h
#define s7nodaveUtilities_h

namespace s7nodave {

/**
 * Splits a string into substrings that are separated by a delimiter.
 */
std::vector<std::string> splitString(const std::string& str, char delimiter);

/**
 * Converts a string to lower case (in place).
 */
void toLower(std::string& str);

/**
 * Converts a string to upper case (in place).
 */
void toUpper(std::string& str);

/**
 * Trims all whitespace on both sides (left and right) of a string (in place).
 */
void trim(std::string& str);

/**
 * Trims all whitespace on the left side of a string (in place).
 */
void trimLeft(std::string& str);

/**
 * Trims all whitespace on the right side of a string (in place).
 */
void trimRight(std::string& str);

} // namespace s7nodave

#endif // s7nodaveUtilities_h
