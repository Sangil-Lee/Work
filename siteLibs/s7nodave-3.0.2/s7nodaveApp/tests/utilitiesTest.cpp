#include "utilities.h"

#include <gtest/gtest.h>

namespace s7nodave {
namespace {

using Strings = std::vector<std::string>;

TEST(UtilitiesTest, testSplitString) {    
    EXPECT_EQ(splitString("", ','), Strings({""}));
    EXPECT_EQ(splitString(",", ','), Strings({"", ""}));
    EXPECT_EQ(splitString(",,", ','), Strings({"", "", ""}));
    EXPECT_EQ(splitString("a,", ','), Strings({"a", ""}));
    EXPECT_EQ(splitString("abc,", ','), Strings({"abc", ""}));
    EXPECT_EQ(splitString(",a", ','), Strings({"", "a"}));
    EXPECT_EQ(splitString(",abc", ','), Strings({"", "abc"}));
    EXPECT_EQ(splitString(",a,", ','), Strings({"", "a", ""}));
    EXPECT_EQ(splitString(",abc,", ','), Strings({"", "abc", ""}));
    EXPECT_EQ(splitString("a,bc,d", ','), Strings({"a", "bc", "d"}));
    EXPECT_EQ(splitString("ab-c,d", '-'), Strings({"ab", "c,d"}));
}

std::string toLowerReturn(const std::string& str) {
    std::string result = str;
    toLower(result);
    return result;
}

TEST(UtilitiesTest, testToLower) {
    EXPECT_EQ(toLowerReturn(""), "");
    EXPECT_EQ(toLowerReturn("abc123"), "abc123");
    EXPECT_EQ(toLowerReturn("ABC123"), "abc123");
    EXPECT_EQ(toLowerReturn("AbC123"), "abc123");
}

std::string toUpperReturn(const std::string& str) {
    std::string result = str;
    toUpper(result);
    return result;
}

TEST(UtilitiesTest, testToUpper) {
    EXPECT_EQ(toUpperReturn(""), "");
    EXPECT_EQ(toUpperReturn("ABC123"), "ABC123");
    EXPECT_EQ(toUpperReturn("abc123"), "ABC123");
    EXPECT_EQ(toUpperReturn("AbC123"), "ABC123");
}

std::string trimReturn(const std::string& str) {
    std::string result = str;
    trim(result);
    return result;
}

TEST(UtilitiesTest, testTrim) {
    EXPECT_EQ(trimReturn(""), "");
    EXPECT_EQ(trimReturn(" "), "");
    EXPECT_EQ(trimReturn(" \t\n\r"), "");
    EXPECT_EQ(trimReturn(" ab c"), "ab c");
    EXPECT_EQ(trimReturn(" d ef  "), "d ef");
    EXPECT_EQ(trimReturn("123 "), "123");
}

std::string trimLeftReturn(const std::string& str) {
    std::string result = str;
    trimLeft(result);
    return result;
}

TEST(UtilitiesTest, testTrimLeft) {
    EXPECT_EQ(trimLeftReturn(""), "");
    EXPECT_EQ(trimLeftReturn(" "), "");
    EXPECT_EQ(trimLeftReturn(" \t\n\r"), "");
    EXPECT_EQ(trimLeftReturn(" ab c"), "ab c");
    EXPECT_EQ(trimLeftReturn(" d ef  "), "d ef  ");
    EXPECT_EQ(trimLeftReturn("123 "), "123 ");
}

std::string trimRightReturn(const std::string& str) {
    std::string result = str;
    trimRight(result);
    return result;
}

TEST(UtilitiesTest, testTrimRight) {
    EXPECT_EQ(trimRightReturn(""), "");
    EXPECT_EQ(trimRightReturn(" "), "");
    EXPECT_EQ(trimRightReturn(" \t\n\r"), "");
    EXPECT_EQ(trimRightReturn(" ab c"), " ab c");
    EXPECT_EQ(trimRightReturn(" d ef  "), " d ef");
    EXPECT_EQ(trimRightReturn("123 "), "123");
}

} // anonymous namespace
} // namespace s7nodave
