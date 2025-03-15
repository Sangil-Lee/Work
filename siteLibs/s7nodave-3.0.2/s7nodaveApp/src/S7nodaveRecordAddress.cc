#include <vector>

#include "utilities.h"

#include "S7nodaveRecordAddress.h"

namespace s7nodave {

std::string S7nodaveRecordAddress::getPortName() const {
    return this->portName;
}

PlcAddress S7nodaveRecordAddress::getPlcAddress() const {
    return this->plcAddress;
}
s7nodavePlcDataType S7nodaveRecordAddress::getPlcDataType() const {
    return this->plcDataType;
}

S7nodaveRecordAddress::DeviceParameters S7nodaveRecordAddress::getDeviceParameters() const {
    return this->recordParameters;
}

namespace {

const std::string whitespace = " \t\n\r\x0b";

Optional<S7nodaveRecordAddress::DeviceParameters> parseDeviceParameters(std::string parameterString) {
    if (parameterString.empty()) {
        return S7nodaveRecordAddress::DeviceParameters();
    }
    S7nodaveRecordAddress::DeviceParameters parameterMap;
    // Device parameters are separated by commas
    auto parameters = splitString(parameterString, ',');
    for (auto& parameterString : parameters) {
        std::string parameterName;
        Optional<std::string> parameterValue;
        size_t equalPos = parameterString.find_first_of('=');
        if (equalPos == std::string::npos) {
            // Parameter without value
            parameterName = parameterString;
            trim(parameterName);
        } else {
            parameterName = parameterString.substr(0, equalPos);
            trim(parameterName);
            parameterValue = parameterString.substr(equalPos + 1);
            trim(*parameterValue);
        }
        // Parameter names must not be the empty string.
        if (parameterName.empty()) {
            return Optional<S7nodaveRecordAddress::DeviceParameters>();
        }
        // Parameter names are case insensitive, so we normalize them to
        // capital characters.
        toUpper(parameterName);
        if (parameterMap.find(parameterName) != parameterMap.end()) {
            // The same parameter appeared twice
            return Optional<S7nodaveRecordAddress::DeviceParameters>();
        }
        // Add parameter to map
        parameterMap.insert(std::make_pair(parameterName, parameterValue));
    }
    return parameterMap;
}

Optional<s7nodavePlcDataType> stringToPlcDataType(std::string typeString) {
    toLower(typeString);
    if (typeString == "bool") {
        return plcDataTypeBool;
    } else if (typeString == "int8") {
        return plcDataTypeInt8;
    } else if (typeString == "uint8") {
        return plcDataTypeUint8;
    } else if (typeString == "int16") {
        return plcDataTypeInt16;
    } else if (typeString == "uint16") {
        return plcDataTypeUint16;
    } else if (typeString == "int32") {
        return plcDataTypeInt32;
    } else if (typeString == "uint32") {
        return plcDataTypeUint32;
    } else if (typeString == "float") {
        return plcDataTypeFloat;
    } else {
        return Optional<s7nodavePlcDataType>();
    }
}

} // anonymous namesapce

std::tuple< std::string, Optional<S7nodaveRecordAddress::DeviceParameters>, Optional<PlcAddress>, Optional<s7nodavePlcDataType>, bool > S7nodaveRecordAddress::parseRecordAddress(std::string addressString) {
    Optional<PlcAddress> emptyPlcAddress;
    Optional<s7nodavePlcDataType> emptyPlcDataType;
    Optional<DeviceParameters> emptyParameterMap;

    trim(addressString);
    size_t pos;
    // Port name ends with opening parenthesis or whitespace
    pos = addressString.find_first_of(whitespace + "(");
    if (pos == std::string::npos) {
        return std::make_tuple(addressString, emptyParameterMap, emptyPlcAddress, emptyPlcDataType, false);
    }
    std::string portName = addressString.substr(0, pos);
    if (portName.size() == 0) {
        return std::make_tuple(portName, emptyParameterMap, emptyPlcAddress, emptyPlcDataType, false);
    }
    addressString = addressString.substr(pos);
    trimLeft(addressString);
    Optional<DeviceParameters> deviceParameterMap;
    // Handle list of optional parameters
    if (addressString.size() >= 1 && addressString[0] == '(') {
        // Find closing parenthesis.
        pos = addressString.find_first_of(')');
        if (pos == std::string::npos) {
            // Unmatched parenthesis.
            return std::make_tuple(portName, emptyParameterMap, emptyPlcAddress, emptyPlcDataType, false);
        }
        deviceParameterMap = parseDeviceParameters(addressString.substr(1, pos - 1));
        if (!deviceParameterMap) {
            // Invalid parameter string
            return std::make_tuple(portName, emptyParameterMap, emptyPlcAddress, emptyPlcDataType, false);
        }
        addressString = addressString.substr(pos + 1);
        trimLeft(addressString);
    } else {
        // No device parameters
        deviceParameterMap = DeviceParameters();
    }
    // PLC address ends with whitespace (or end of string)
    pos = addressString.find_first_of(whitespace);
    std::string plcAddressString;
    if (pos == std::string::npos) {
        plcAddressString = addressString;
        addressString = "";
    } else {
        plcAddressString = addressString.substr(0, pos);
        addressString = addressString.substr(pos);
        trimLeft(addressString);
    }
    Optional<PlcAddress> plcAddress = PlcAddress::create(plcAddressString);
    if (!plcAddress) {
        // Invalid PLC address
        return std::make_tuple(portName, deviceParameterMap, emptyPlcAddress, emptyPlcDataType, false);
    }
    // The rest of the string is the PLC data type
    Optional<s7nodavePlcDataType> plcDataType;
    if (addressString.size() > 0) {
        plcDataType = stringToPlcDataType(addressString);
        if (plcDataType) {
            return std::make_tuple(portName, deviceParameterMap, plcAddress, plcDataType, true);
        } else {
            return std::make_tuple(portName, deviceParameterMap, plcAddress, emptyPlcDataType, false);
        }
    } else {
        return std::make_tuple(portName, deviceParameterMap, plcAddress, emptyPlcDataType, true);
    }
}

Optional<S7nodaveRecordAddress> S7nodaveRecordAddress::create(std::string portName, PlcAddress plcAddress, s7nodavePlcDataType plcDataType, DeviceParameters recordParameters) {
    Optional<S7nodaveRecordAddress> emptyResult;
    // Port name must not be empty
    if (portName.size() == 0) {
        return emptyResult;
    }
    // If a PLC data type is specified, its size has to match the data size of
    // the PLC address.
    int dataSize = PlcAddress::dataSizeInBits(plcAddress.getDataSize());
    switch (plcDataType) {
    case plcDataTypeBool:
        if (dataSize != 1) {
            return emptyResult;
        }
        break;
    case plcDataTypeInt8:
    case plcDataTypeUint8:
        if (dataSize != 8) {
            return emptyResult;
        }
        break;
    case plcDataTypeInt16:
    case plcDataTypeUint16:
        if (dataSize != 16) {
            return emptyResult;
        }
        break;
    case plcDataTypeInt32:
    case plcDataTypeUint32:
    case plcDataTypeFloat:
        if (dataSize != 32) {
            return emptyResult;
        }
        break;
    }
    return S7nodaveRecordAddress(portName, plcAddress, plcDataType, recordParameters);
}

S7nodaveRecordAddress::S7nodaveRecordAddress(std::string portName, PlcAddress plcAddress, s7nodavePlcDataType plcDataType, DeviceParameters recordParameters) :
portName(portName), plcAddress(plcAddress), plcDataType(plcDataType), recordParameters(recordParameters) {
}

} // namespace s7nodave
