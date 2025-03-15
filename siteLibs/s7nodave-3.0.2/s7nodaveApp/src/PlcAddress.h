#ifndef s7nodave_PlcAddress_h
#define s7nodave_PlcAddress_h

#include <string>

#include "Optional.h"
#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Represents the memory address in a PLC.
 */
class PlcAddress {
public:
    /**
     * Tells whether this address is the same as another address.
     */
    bool operator==(const PlcAddress& other) const;

    /**
     * Returns the area of the PLC memory this address refers to.
     */
    s7nodavePlcArea getArea() const;

    /**
     * Returns the area number of the PLC address. This is always zero for all
     * area types except "DB".
     */
    int getAreaNumber() const;

    /**
     * Returns the size of the element this memory address refers to (bit, byte,
     * word or dword).
     */
    s7nodavePlcDataSize getDataSize() const;

    /**
     * Returns the number of the first byte this address refers to.
     */
    int getStartByte() const;

    /**
     * Returns the number of the first bit within the first byte this address
     * refers to. This is always zero, except for addresses refering to a
     * single bit.
     */
    char getStartBit() const;

    /**
     * Returns a string representation of this address (e.g. "DB1.DBW4",
     * "I0.2" or "QB1".
     */
    std::string toString() const;

    /**
     * Returns a string representing the specified area (e.g. "I", "Q", "DB").
     */
    static std::string areaToString(s7nodavePlcArea area);

    /**
     * Returns the area corresponding to the specified string or an empty result
     * if the string does not represent a recognized area type. The recognized
     * strings are "I" or "E" for the input area, "Q" or "A" for the output
     * area, "DB" for the data-block area, "F" or "M" for the flag area, "T" for
     * the timer area and "C" or "Z" for the counter area. The string comparison
     * is not case sensitive.
     */
    static Optional<s7nodavePlcArea> stringToArea(std::string areaString);

    /**
     * Returns the string corresponding to the specified data size (e.g. "B" for
     * byte). If the address is within a DB block, "X" is returned for bit
     * addresses. Otherwise, an empty string is returned for a bit address.
     */
    static std::string dataSizeToString(s7nodavePlcDataSize dataSize, bool isDB);

    /**
     * Returns the data size corresponding to the specified string or the empty
     * result if the string does not represent a recognized data size. The
     * recognized strings are the empty string ("") for a bit, "B" for a byte
     * "W" for a word and "D" for a dword. The string comparison is not case
     * sensitive. For a DB block, "X" is expected for a bit address instead of
     * the usual empty string.
     */
    static Optional<s7nodavePlcDataSize> stringToDataSize(std::string dataSizeString, bool isDB);

    /**
     * Represents the number of bits for the specified data-size (e.g. 8 for a
     * byte).
     */
    static int dataSizeInBits(s7nodavePlcDataSize dataSize);

    /**
     * Creates a PLC address from the specified parameters. If the specified
     * parameters are not valid (e.g. a non-zero areaNumber is specified for
     * the input area), an empty result is returned.
     */
    static Optional<PlcAddress> create(s7nodavePlcArea area, int areaNumber, s7nodavePlcDataSize dataSize, int startByte, int startBit);

    /**
     * Creates a PLC address from a string representing a PLC address. If the
     * specified string does not represent a valid PLC memory address, the
     * empty result is returned.
     */
    static Optional<PlcAddress> create(std::string addressString);

protected:
    /**
     * Protected Constructor. The static create methods should be used instead
     * for creating instances of this class.
     */
    PlcAddress(s7nodavePlcArea area, int areaNumber, s7nodavePlcDataSize, int startByte, char startBit);

    /**
     * Stores the PLC memory area type.
     */
    s7nodavePlcArea area;

    /**
     * Stores the PLC memory area number (for DB-type areas).
     */
    int areaNumber;

    /**
     * Stores the PLC data-size of the memory address.
     */
    s7nodavePlcDataSize dataSize;

    /**
     * Stores the start byte of the memory address.
     */
    int startByte;

    /**
     * Store the start bit of the memory address (for bit-addresses only).
     */
    char startBit;
};

} // namespace s7nodave

#endif // s7nodave_PlcAddress_h
