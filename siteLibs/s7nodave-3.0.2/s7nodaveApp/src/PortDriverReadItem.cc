#include <string.h>

#include "PortDriverReadItem.h"

namespace s7nodave {

using ReadItem = PortDriverReadItem;
using ReadItemPtr = std::shared_ptr<PortDriverReadItem>;
using ReadItemList = std::list<ReadItemPtr>;

ReadItem::PortDriverReadItem(s7nodavePlcArea area, int areaNumber, int startByte, int readBytes) :
    area(area),
    areaNumber(areaNumber),
    startByte(startByte),
    readBytes(readBytes),
    bytesReadReference(nullptr),
    bytesReadInternal(0),
    statusReference(nullptr),
    statusInternal(asynSuccess),
    buffer(nullptr),
    // We use the internal buffer, because none was passed.
    usingInternalBuffer(true) {
    // Reserve space for the buffer
    buffer = pasynManager->memMalloc(this->readBytes);
};

ReadItem::PortDriverReadItem(asynS7nodaveReadItem *readItem) :
    area(readItem->area),
    areaNumber(readItem->areaNumber),
    startByte(readItem->startByte),
    readBytes(readItem->readBytes),
    bytesReadReference(&readItem->bytesRead),
    bytesReadInternal(0),
    statusReference(&readItem->status),
    statusInternal(asynSuccess),
    buffer(readItem->buffer),
    usingInternalBuffer(false) {
};

ReadItem::~PortDriverReadItem() {
    if (usingInternalBuffer && buffer) {
        pasynManager->memFree(this->buffer, this->readBytes);
        this->buffer = nullptr;
    }
}

bool ReadItem::addReadItems(std::list<std::shared_ptr<PortDriverReadItem>> items) {
    int newStartByte = this->startByte;
    int newEndByte = this->getEndByte();
    for (auto& item : items) {
        newStartByte = std::min(newStartByte, item->getStartByte());
        newEndByte = std::max(newEndByte, item->getEndByte());
    }
    int newLength = newEndByte - newStartByte;
    if (newStartByte != this->startByte || newLength != this->readBytes) {
        if (!usingInternalBuffer || bytesReadInternal != 0) {
            return false;
        }
        // Resize buffer
        void *newBuffer = pasynManager->memMalloc(newLength);
        if (!newBuffer) {
            return false;
        }
        pasynManager->memFree(this->buffer, this->readBytes);
        this->buffer = newBuffer;
        this->startByte = newStartByte;
        this->readBytes = newLength;
    }
    // Insert all items into list
    this->includedReadItems.splice(this->includedReadItems.end(), items);
    return true;
}

bool ReadItem::addReadItem(std::shared_ptr<PortDriverReadItem> item) {
    if (!item) {
        return false;
    }
    if (this->area != item->area || this->areaNumber != item->areaNumber) {
        return false;
    }
    int newStartByte = std::min(this->startByte, item->startByte);
    int newEndByte = std::max(this->getEndByte(), item->getEndByte());
    int newLength = newEndByte - newStartByte;
    if (newStartByte != this->startByte || newLength != this->readBytes) {
        if (!usingInternalBuffer || bytesReadInternal != 0) {
            return false;
        }
        // Resize buffer
        void *newBuffer = pasynManager->memMalloc(newLength);
        if (!newBuffer) {
            return false;
        }
        pasynManager->memFree(this->buffer, this->readBytes);
        this->buffer = newBuffer;
        this->startByte = newStartByte;
        this->readBytes = newLength;
    }
    this->includedReadItems.push_back(item);
    return true;
}

void ReadItem::dispatchReadResultToChildren() {
    for (auto& child : this->includedReadItems) {
        // Copy buffer to target item
        void *buffer1 = this->buffer;
        void *buffer2 = child->getBuffer();
        int bytesRead =
            this->usingInternalBuffer
            ? this->bytesReadInternal : *(this->bytesReadReference);
        asynStatus status =
            this->usingInternalBuffer
            ? this->statusInternal : *(this->statusReference);
        if (status == asynSuccess) {
            if (bytesRead != 0 && buffer1 && buffer2) {
                // There might be an offset between the buffer in this item and
                // the start of the range the child item is interested in.
                int offset = (child->getStartByte() - this->startByte);
                // If an error occurred, there might be less bytes available in
                // buffer1 than asked for by buffer2. In this case, we only read
                // the available bytes.
                bytesRead = std::min(bytesRead - offset, child->getReadBytes());
                // We have to set the number of bytes read in the child item.
                child->setBytesRead(bytesRead);
                // memcpy does not accept an offset, so we have to change the
                // pointer accordingly.
                void *source = static_cast<void *>(static_cast<char *>(buffer1) + offset);
                memcpy(buffer2, source, bytesRead);
            } else {
                child->setBytesRead(0);
            }
        } else {
            child->setErrorStatus(status);
        }
        // Process items included by the processed child item.
        child->dispatchReadResultToChildren();
    }
}

bool ReadItem::includesNoAreaCheck(const ReadItem& other) const {
    if (this->startByte > other.startByte) {
        return false;
    }
    if (this->getEndByte() < other.getEndByte()) {
        return false;
    }
    return true;
}

bool ReadItem::compareStartByte(const ReadItem& first, const ReadItem& second) {
    if (first.startByte < second.startByte) {
        return true;
    } else {
        return false;
    }
}

bool ReadItem::pointerCompareStartByte(const std::shared_ptr<PortDriverReadItem>& first, const std::shared_ptr<PortDriverReadItem>& second) {
    if (!first && second) {
        return true;
    } else if ((first && !second) || (!first && !second)) {
        return false;
    } else {
        return compareStartByte(*first, *second);
    }
}

} // namespace s7nodave
