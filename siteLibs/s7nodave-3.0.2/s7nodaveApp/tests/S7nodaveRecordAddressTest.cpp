#include "S7nodaveRecordAddress.h"

#include <gtest/gtest.h>

namespace s7nodave {
namespace {

TEST(S7nodaveRecordAddressTest, testCreate) {
    Optional<S7nodaveRecordAddress> o;
    S7nodaveRecordAddress::DeviceParameters params;
    auto optionalPlcAddr = PlcAddress::create("DB2.DBB3");
    ASSERT_TRUE(optionalPlcAddr);
    auto plcAddr = *optionalPlcAddr;
    o = S7nodaveRecordAddress::create("AbC", plcAddr, plcDataTypeUint8, params);
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getPortName(), "AbC");
    EXPECT_EQ(o->getPlcAddress(), plcAddr);
    EXPECT_EQ(o->getPlcDataType(), plcDataTypeUint8);
    EXPECT_EQ(o->getDeviceParameters(), params);
    optionalPlcAddr = PlcAddress::create("FD4");
    ASSERT_TRUE(optionalPlcAddr);
    plcAddr = *optionalPlcAddr;
    params.emplace("name", "value");
    o = S7nodaveRecordAddress::create("def", plcAddr, plcDataTypeInt32, params);
    ASSERT_TRUE(o);
    EXPECT_EQ(o->getPortName(), "def");
    EXPECT_EQ(o->getPlcAddress(), plcAddr);
    EXPECT_EQ(o->getPlcDataType(), plcDataTypeInt32);
    EXPECT_EQ(o->getDeviceParameters(), params);
    // If the port name is empty, the address cannot be created.
    o = S7nodaveRecordAddress::create("", plcAddr, plcDataTypeInt32, params);
    EXPECT_FALSE(o);
    // If the width specified by the PLC address does not match the data type,
    // the address cannot be created.
    o = S7nodaveRecordAddress::create("def", plcAddr, plcDataTypeInt16, params);
    EXPECT_FALSE(o);
}

TEST(S7nodaveRecordAddressTest, testParseRecordAddress) {
    std::string port;
    Optional<S7nodaveRecordAddress::DeviceParameters> deviceParameters;
    Optional<S7nodaveRecordAddress::DeviceParameters> expectedDeviceParameters =
        Optional<S7nodaveRecordAddress::DeviceParameters>(
            S7nodaveRecordAddress::DeviceParameters());
    ASSERT_TRUE(expectedDeviceParameters);
    Optional<PlcAddress> plcAddress;
    Optional<s7nodavePlcDataType> dataType;
    bool success;
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "Port0 (aBc=deF,ghi=jkl) QD4 int32");
    EXPECT_EQ(port, "Port0");
    expectedDeviceParameters->clear();
    expectedDeviceParameters->emplace("ABC", "deF");
    expectedDeviceParameters->emplace("GHI", "jkl");
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "Port0(  abc =deF ,GHI =jkl)QD4 int32");
    EXPECT_EQ(port, "Port0");
    expectedDeviceParameters->clear();
    expectedDeviceParameters->emplace("ABC", "deF");
    expectedDeviceParameters->emplace("GHI", "jkl");
    EXPECT_EQ(deviceParameters, expectedDeviceParameters);
    EXPECT_EQ(plcAddress, PlcAddress::create("QD4"));
    EXPECT_EQ(dataType, plcDataTypeInt32);
    EXPECT_TRUE(success);
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "some_port () FB3");
    EXPECT_EQ(port, "some_port");
    expectedDeviceParameters->clear();
    EXPECT_EQ(deviceParameters, expectedDeviceParameters);
    EXPECT_EQ(plcAddress, PlcAddress::create("FB3"));
    EXPECT_FALSE(dataType);
    EXPECT_TRUE(success);
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "some_port FB3");
    EXPECT_EQ(port, "some_port");
    expectedDeviceParameters->clear();
    EXPECT_EQ(deviceParameters, expectedDeviceParameters);
    EXPECT_EQ(plcAddress, PlcAddress::create("FB3"));
    EXPECT_FALSE(dataType);
    EXPECT_TRUE(success);
    // We do not support empty parameter names.
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "Port0 (=abc) QD4 int32");
    EXPECT_FALSE(deviceParameters);
    EXPECT_FALSE(success);
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "Port0 (ABC=123,) QD4 int32");
    EXPECT_FALSE(deviceParameters);
    EXPECT_FALSE(success);
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "Port0 (,ABC=123) QD4 int32");
    EXPECT_FALSE(deviceParameters);
    EXPECT_FALSE(success);
    std::tie(port, deviceParameters, plcAddress, dataType, success) =
        S7nodaveRecordAddress::parseRecordAddress(
            "Port0 (,) QD4 int32");
    EXPECT_FALSE(deviceParameters);
    EXPECT_FALSE(success);
}

} // anonymous namespace
} // namespace s7nodave
