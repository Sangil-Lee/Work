#ifndef S7nodaveRecordAddress_h
#define S7nodaveRecordAddress_h

#include <map>
#include <string>
#include <tuple>

#include "s7nodaveAsyn.h"

#include "Optional.h"
#include "PlcAddress.h"

namespace s7nodave {

/**
 * Represents the whole device address given for a record.
 */
class S7nodaveRecordAddress {
public:
    using DeviceParameters = std::map<std::string, Optional<std::string>>;

    /**
     * Returns the port name of the device.
     */
    std::string getPortName() const;

    /**
     * Returns the PLC memory address.
     */
    PlcAddress getPlcAddress() const;

    /**
     * Returns the PLC data-type.
     */
    s7nodavePlcDataType getPlcDataType() const;

    /**
     * Returns the optional device parameters given.
     */
    DeviceParameters getDeviceParameters() const;

    /**
     * Parses a device address string and returns  the various components a
     * record's device address is made of. If an error occurs while parsing the
     * string, the last element of the returned tuple is set to false.
     * The tuple contains:
     * - the port name
     * - device parameters
     * - the PLC memory address
     * - the PLC data-type
     * - a boolean flag that is set to true if the parsing process was
     *   successful and set to false if an error occurred.
     * If the success flag is true, all tuple elements except the PLC data-type
     * are guaranteed to be set. The PLC data-type is only set if it has been
     * specified in the record's device address string.
     */
    static std::tuple< std::string, Optional<DeviceParameters>, Optional<PlcAddress>, Optional<s7nodavePlcDataType>, bool > parseRecordAddress(std::string addressString);

    /**
     * Creates a record device address. If the parameters passed to this method
     * are invalid, an empty result is returned.
     */
    static Optional<S7nodaveRecordAddress> create(std::string portName, PlcAddress plcAddress, s7nodavePlcDataType plcDataType, DeviceParameters deviceParameters);

protected:
    /**
     * Protected constructor. The static create method should be used instead
     * for creating an instance of this class.
     */
    S7nodaveRecordAddress(std::string portName, PlcAddress plcAddress, s7nodavePlcDataType plcDataType, DeviceParameters deviceParameters);

    /**
     * Device's port name.
     */
    std::string portName;

    /**
     * PLC memory address.
     */
    PlcAddress plcAddress;

    /**
     * PLC data-type.
     */
    s7nodavePlcDataType plcDataType;

    /**
     * Optional device parameters.
     */
    DeviceParameters recordParameters;
};

} // namespace s7nodave

#endif // S7nodaveRecordAddress_h
