#include "PlcAddress.h"

#include <gtest/gtest.h>

namespace s7nodave {
namespace {

TEST(PlcAddressTest, testCreate) {
    Optional<PlcAddress> o =
        PlcAddress::create(plcAreaDb, 5, plcDataSizeBit, 15, 2);
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaDb);
    EXPECT_EQ(o->getAreaNumber(), 5);
    EXPECT_EQ(o->getDataSize(), plcDataSizeBit);
    EXPECT_EQ(o->getStartByte(), 15);
    EXPECT_EQ(o->getStartBit(), 2);
    o = PlcAddress::create(plcAreaInputs, 0, plcDataSizeDword, 42, 0);
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaInputs);
    EXPECT_EQ(o->getAreaNumber(), 0);
    EXPECT_EQ(o->getDataSize(), plcDataSizeDword);
    EXPECT_EQ(o->getStartByte(), 42);
    EXPECT_EQ(o->getStartBit(), 0);
    // If the PLC area is not DB, the area number must be zero.
    o = PlcAddress::create(plcAreaInputs, 2, plcDataSizeDword, 42, 0);
    EXPECT_FALSE(o);
    o = PlcAddress::create(plcAreaFlags, 11, plcDataSizeWord, 23, 0);
    EXPECT_FALSE(o);
    // The start byte must be non-negative.
    o = PlcAddress::create(plcAreaDb, 5, plcDataSizeBit, -1, 2);
    EXPECT_FALSE(o);
    o = PlcAddress::create(plcAreaFlags, 0, plcDataSizeByte, -5, 0);
    EXPECT_FALSE(o);
    // The start bit must be non-negative and less than eight.
    o = PlcAddress::create(plcAreaDb, 5, plcDataSizeBit, 15, -1);
    EXPECT_FALSE(o);
    o = PlcAddress::create(plcAreaDb, 5, plcDataSizeBit, 15, 8);
    EXPECT_FALSE(o);
    // The start bit must be zero if the data size is not bit.
    o = PlcAddress::create(plcAreaDb, 5, plcDataSizeByte, 15, 2);
    EXPECT_FALSE(o);
    o = PlcAddress::create(plcAreaOutputs, 5, plcDataSizeDword, 33, 7);
    EXPECT_FALSE(o);
}

TEST(PlcAddressTest, testCreateFromString) {
    Optional<PlcAddress> o = PlcAddress::create("DB7.DBW3");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaDb);
    EXPECT_EQ(o->getAreaNumber(), 7);
    EXPECT_EQ(o->getDataSize(), plcDataSizeWord);
    EXPECT_EQ(o->getStartByte(), 3);
    EXPECT_EQ(o->getStartBit(), 0);
    o = PlcAddress::create("DB9.DBX2.1");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaDb);
    EXPECT_EQ(o->getAreaNumber(), 9);
    EXPECT_EQ(o->getDataSize(), plcDataSizeBit);
    EXPECT_EQ(o->getStartByte(), 2);
    EXPECT_EQ(o->getStartBit(), 1);
    o = PlcAddress::create("F5.1");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaFlags);
    EXPECT_EQ(o->getAreaNumber(), 0);
    EXPECT_EQ(o->getDataSize(), plcDataSizeBit);
    EXPECT_EQ(o->getStartByte(), 5);
    EXPECT_EQ(o->getStartBit(), 1);
    o = PlcAddress::create("ID4");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaInputs);
    EXPECT_EQ(o->getAreaNumber(), 0);
    EXPECT_EQ(o->getDataSize(), plcDataSizeDword);
    EXPECT_EQ(o->getStartByte(), 4);
    EXPECT_EQ(o->getStartBit(), 0);
    o = PlcAddress::create("QB7");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaOutputs);
    EXPECT_EQ(o->getAreaNumber(), 0);
    EXPECT_EQ(o->getDataSize(), plcDataSizeByte);
    EXPECT_EQ(o->getStartByte(), 7);
    EXPECT_EQ(o->getStartBit(), 0);
    o = PlcAddress::create("C8");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaCounter);
    EXPECT_EQ(o->getAreaNumber(), 0);
    EXPECT_EQ(o->getDataSize(), plcDataSizeWord);
    EXPECT_EQ(o->getStartByte(), 8);
    EXPECT_EQ(o->getStartBit(), 0);
    o = PlcAddress::create("T4");
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getArea(), plcAreaTimer);
    EXPECT_EQ(o->getAreaNumber(), 0);
    EXPECT_EQ(o->getDataSize(), plcDataSizeWord);
    EXPECT_EQ(o->getStartByte(), 4);
    EXPECT_EQ(o->getStartBit(), 0);
    // When specifying a bit inside a DB area, there must be an X for the data
    // type.
    o = PlcAddress::create("DB9.DB2.1");
    EXPECT_FALSE(o);
    // When specifying a bit in another area, there must be no X for the data
    // type.
    o = PlcAddress::create("IX2.1");
    EXPECT_FALSE(o);
    // A non-bit address must not have a bit part.
    o = PlcAddress::create("DB9.DBW2.1");
    EXPECT_FALSE(o);
    // A bit address must have a bit part.
    o = PlcAddress::create("DB9.DBX2");
    EXPECT_FALSE(o);
    // V is not a valid area identifier.
    o = PlcAddress::create("VW4");
    EXPECT_FALSE(o);
    // Counters do not specify a data type.
    o = PlcAddress::create("CW8");
    EXPECT_FALSE(o);
    // Timers do not specify a data type.
    o = PlcAddress::create("TW4");
    EXPECT_FALSE(o);
}

TEST(PlcAddressTest, testDataSizeInBits) {
    EXPECT_EQ(PlcAddress::dataSizeInBits(plcDataSizeBit), 1);
    EXPECT_EQ(PlcAddress::dataSizeInBits(plcDataSizeByte), 8);
    EXPECT_EQ(PlcAddress::dataSizeInBits(plcDataSizeWord), 16);
    EXPECT_EQ(PlcAddress::dataSizeInBits(plcDataSizeDword), 32);
}

TEST(PlcAddressTest, testDataSizeToString) {
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeBit, true), "X");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeBit, false), "");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeByte, true), "B");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeByte, false), "B");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeWord, true), "W");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeWord, false), "W");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeDword, true), "D");
    EXPECT_EQ(PlcAddress::dataSizeToString(plcDataSizeDword, false), "D");
}
TEST(PlcAddressTest, testEqualityOperator) {
    Optional<PlcAddress> o1 = PlcAddress::create("I4.2");
    Optional<PlcAddress> o2 = PlcAddress::create("DB2.DBW4");
    ASSERT_TRUE(o1);
    ASSERT_TRUE(o2);
    EXPECT_EQ(*o1, *o1);
    EXPECT_EQ(*o2, *o2);
    // We do not use EXPECT_NE here, because we do not implement operator!=.
    EXPECT_FALSE(*o1 == *o2);
    EXPECT_FALSE(*o2 == *o1);
}

TEST(PlcAddressTest, testStringToArea) {
    Optional<s7nodavePlcArea> o = PlcAddress::stringToArea("C");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaCounter);
    o = PlcAddress::stringToArea("Z");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaCounter);
    o = PlcAddress::stringToArea("DB");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaDb);
    o = PlcAddress::stringToArea("F");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaFlags);
    o = PlcAddress::stringToArea("M");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaFlags);
    o = PlcAddress::stringToArea("E");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaInputs);
    o = PlcAddress::stringToArea("I");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaInputs);
    o = PlcAddress::stringToArea("A");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaOutputs);
    o = PlcAddress::stringToArea("Q");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaOutputs);
    o = PlcAddress::stringToArea("T");
    ASSERT_TRUE(o);
    ASSERT_EQ(*o, plcAreaTimer);
    // X is not a valid area.
    o = PlcAddress::stringToArea("X");
    EXPECT_FALSE(o);
}

TEST(PlcAddressTest, testStringToDataSize) {
    Optional<s7nodavePlcDataSize> o = PlcAddress::stringToDataSize("X", true);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeBit);
    o = PlcAddress::stringToDataSize("", false);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeBit);
    o = PlcAddress::stringToDataSize("B", true);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeByte);
    o = PlcAddress::stringToDataSize("B", false);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeByte);
    o = PlcAddress::stringToDataSize("W", true);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeWord);
    o = PlcAddress::stringToDataSize("W", false);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeWord);
    o = PlcAddress::stringToDataSize("D", true);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeDword);
    o = PlcAddress::stringToDataSize("D", false);
    ASSERT_TRUE(o);
    EXPECT_EQ(*o, plcDataSizeDword);
    // In the DB area, an empty string for the data type is not valid.
    o = PlcAddress::stringToDataSize("", true);
    EXPECT_FALSE(o);
    // Outside the DB area, an X for the data type is not valid.
    o = PlcAddress::stringToDataSize("X", false);
    EXPECT_FALSE(o);
}

TEST(PlcAddressTest, testToString) {
    std::vector<std::string> testAddresses{
        "C7",
        "C3",
        "DB7.DBX3.1",
        "DB2.DBB4",
        "DB0.DBW12",
        "DB11.DBD16",
        "F12.1",
        "FB2",
        "FW4",
        "FD8",
        "I7.1",
        "IB0",
        "IW2",
        "ID8",
        "T0",
        "T9",
        "Q9.7",
        "QB37",
        "QW42",
        "QD100"};
    for (auto& addressStr : testAddresses) {
        Optional<PlcAddress> o = PlcAddress::create(addressStr);
        ASSERT_TRUE(o);
        EXPECT_EQ(o->toString(), addressStr);
    }
}

} // anonymous namespace
} // namespace s7nodave
