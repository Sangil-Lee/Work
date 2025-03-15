#ifndef s7nodave_PortDriverReadItem_h
#define s7nodave_PortDriverReadItem_h

#include <list>
#include <memory>

#include "s7nodaveAsyn.h"

namespace s7nodave {

/**
 * Represents an item that should be read (or has been read) from the PLC.
 * This class is intended for use by S7nodavePortDriver and
 * PortDriverReadOptimizer only.
 */
class PortDriverReadItem {
public:
    /**
     * Constructor. Constructs a new item that uses an internal buffer using
     * the specified PLC memory address.
     */
    PortDriverReadItem(s7nodavePlcArea area, int areaNumber, int startByte, int readBytes);

    /**
     * Constructor. Constructs a new item that is a wrapper around an
     * asynS7nodaveReadItem and its buffer.
     */
    PortDriverReadItem(asynS7nodaveReadItem *readItem);

    /**
     * Destructor.
     */
    ~PortDriverReadItem();

    /**
     * Returns the PLC area type used.
     */
    inline s7nodavePlcArea getArea() const {
        return this->area;
    };

    /**
     * Returns the PLC area number.
     */
    inline int getAreaNumber() const {
        return this->areaNumber;
    };

    /**
     * Returns the start byte of the address.
     */
    inline int getStartByte() const {
        return this->startByte;
    };

    /**
     * Returns the number of bytes that shall be read.
     */
    inline int getReadBytes() const {
        return this->readBytes;
    };

    /**
     * Returns the number of the first byte that is NOT in the read range.
     */
    inline int getEndByte() const {
        return this->startByte + this->readBytes;
    };

    /**
     * Returns the number of bytes that have been read.
     */
    inline int getBytesRead() const {
        if (usingInternalBuffer) {
            return this->bytesReadInternal;
        } else {
            // Use the bytesRead field in the underlying asynS7nodaveReadItem
            // structure.
            return *(this->bytesReadReference);
        }
    };

    /**
     * Returns the status code that indicates the result of the read operation
     * for this item.
     */
    inline asynStatus getErrorStatus() const {
        if (usingInternalBuffer) {
            return this->statusInternal;
        } else {
            // Use the bytesRead field in the underlying asynS7nodaveReadItem
            // structure.
            return *(this->statusReference);
        }
    };

    /**
     * Sets the number of bytes that have been read. This also sets the status
     * to asynSuccess.
     */
    inline void setBytesRead(int bytesRead) {
        if (usingInternalBuffer) {
            this->bytesReadInternal = bytesRead;
            this->statusInternal = asynSuccess;
        } else {
            // Set the bytesRead field in the underlying asynS7nodaveReadItem
            // structure.
            *this->bytesReadReference = bytesRead;
            *this->statusReference = asynSuccess;
        }
    };

    /**
     * Sets the status that represents the read result to the specified error
     * code. This also sets the number of bytes read to zero.
     */
    inline void setErrorStatus(asynStatus status) {
        // This method must not be called if there is no error.
        assert(status != asynSuccess);
        if (usingInternalBuffer) {
            this->bytesReadInternal = 0;
            this->statusInternal = status;
        } else {
            // Set the bytesRead field in the underlying asynS7nodaveReadItem
            // structure.
            *this->bytesReadReference = 0;
            *this->statusReference = status;
        }
    }

    /**
     * Returns the buffer that contains the data read from the PLC. If the
     * buffer has been allocated by this item, the returned pointer is only
     * valid as long as this object exists.
     */
    inline void *getBuffer() const {
        return buffer;
    };

    /**
     * Adds items that should be included in this item. This only works if the
     * items specify the same memory area as this item. This item is resized
     * to span the whole range of all items contained, if needed. If this item
     * cannot be resized (e.g. because it is backed by an asynS7nodaveReadItem
     * structure), false is returned and the items are not added.
     */
    bool addReadItems(std::list<std::shared_ptr<PortDriverReadItem>> items);

    /**
     * Add an item that should be included in this item. This only works if the
     * item specifies the same memory area as this item. This item is resized
     * to span the whole range of both items, if needed. If this item cannot be
     * resized (e.g. because it is backed by an asynS7nodaveReadItem structure),
     * false is returned and the item is not added.
     */
    bool addReadItem(std::shared_ptr<PortDriverReadItem> item);

    /**
     * Copies the content of this item's buffer to the children's buffers and
     * their children's buffers, etc.
     */
    void dispatchReadResultToChildren();

    /**
     * Checks whether this item includes the whole memory range of the passed
     * item. This method does not check that both items actually refer to the
     * same memory area.
     */
    bool includesNoAreaCheck(const PortDriverReadItem& otherItem) const;

    /**
     * Compares the start byte of two items. Can be used to sort a list of items
     * by their start addresses. This method does not compare the memory area.
     */
    static bool compareStartByte(const PortDriverReadItem& first, const PortDriverReadItem& second);

    /**
     * Wrapper around compareStartByte that can be used when the two items to be
     * compared are both wrapped in shared_ptr containers.
     */
    static bool pointerCompareStartByte(const std::shared_ptr<PortDriverReadItem>& first, const std::shared_ptr<PortDriverReadItem>& second);

private:
    /*
     * Private copy constructor. Objects of this class should never be copied.
     */
    PortDriverReadItem(const PortDriverReadItem &);

    /**
     * Private assignment operator. Objects of this class should never be
     * copied.
     */
    PortDriverReadItem& operator=(const PortDriverReadItem &);

    /**
     * Items included in this item.
     */

    std::list<std::shared_ptr<PortDriverReadItem>> includedReadItems;

    /**
     * PLC memory area type.
     */
    s7nodavePlcArea area;

    /**
     * PLC memory area number.
     */
    int areaNumber;

    /**
     * PLC address start byte.
     */
    int startByte;

    /**
     * Number of bytes that shall be read from the PLC.
     */
    int readBytes;

    /**
     * Pointer to the bytesRead field of the underlying asynS7nodaveReadItem
     * structure or NULL, if this object has been constructed without using
     * such a structure.
     */
    int *bytesReadReference;

    /**
     * Number of bytes read. Only used if this object has been constructed
     * without using an asynS7nodaveReadItem structure.
     */
    int bytesReadInternal;

    /**
     * Pointer to the status field of the underlying asynS7nodaveReadItem
     * structure or NULL, if this object has been constructed without using
     * such a structure.
     */
    asynStatus *statusReference;

    /**
     * Status associated with the read result. Only used if this object has been
     * constructed without using an asynS7nodaveReadItem structure.
     */
    asynStatus statusInternal;

    /**
     * Buffer for storing read data. Might either be allocated internally or
     * a reference to the buffer of an asynS7nodaveReadItem structure.
     */
    void *buffer;

    /**
     * True if buffer has been allocated internally.
     */
    bool usingInternalBuffer;
};

} // namespace s7nodave

#endif // s7nodave_PortDriverReadItem_h
