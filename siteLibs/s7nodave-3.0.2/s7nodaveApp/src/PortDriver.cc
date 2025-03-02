#include <string>

#include <fcntl.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <asynDriver.h>
#include <epicsGuard.h>

#include "PlcAddress.h"
#include "PortDriverReadOptimizer.h"

#include "PortDriver.h"

namespace s7nodave {

using ReadItemList = PortDriverReadOptimizer::ReadItemList;

PortDriver::PortDriver(const char *portName, const char *plcHostname, const int plcPort, const int plcRack, const int plcSlot, const unsigned int priority) {
    this->connected = false;
    this->portName = std::string(portName);
    this->plcHostname = std::string(plcHostname);
    this->plcPort = plcPort;
    this->plcRack = plcRack;
    this->plcSlot = plcSlot;
    this->priority = priority;
    this->registered = false;

    // Initialize asyn structures
    this->myAsynCommon.report = asynReportStatic;
    this->myAsynCommon.connect = asynConnectStatic;
    this->myAsynCommon.disconnect = asynDisconnectStatic;
    this->myAsynCommonInterface.interfaceType = asynCommonType;
    this->myAsynCommonInterface.pinterface = &this->myAsynCommon;
    this->myAsynCommonInterface.drvPvt = this;
    this->myAsynS7nodave.plcRead = asynPlcReadStatic;
    this->myAsynS7nodave.plcReadMultipleItems = asynPlcReadMultipleItemsStatic;
    this->myAsynS7nodave.plcWrite = asynPlcWriteStatic;
    this->myAsynS7nodave.plcWriteBit = asynPlcWriteBitStatic;
    this->myAsynS7nodaveInterface.interfaceType = asynS7nodaveType;
    this->myAsynS7nodaveInterface.pinterface = &this->myAsynS7nodave;
    this->myAsynS7nodaveInterface.drvPvt = this;
}

PortDriver::~PortDriver() {
}

asynStatus PortDriver::registerPortDriver() {
    // Interfaces should only be registered once.
    if (this->registered) {
        return asynSuccess;
    }

    asynStatus status = pasynManager->registerPort(portName.c_str(), ASYN_CANBLOCK, 1, this->priority, 0);
    if (status != asynSuccess) {
        return status;
    }

    status = pasynManager->registerInterface(this->portName.c_str(), &this->myAsynCommonInterface);
    if (status != asynSuccess) {
        return status;
    }

    status = pasynManager->registerInterface(this->portName.c_str(), &this->myAsynS7nodaveInterface);
    if (status != asynSuccess) {
        return status;
    }

    this->registered = true;
    return asynSuccess;
}

void PortDriver::asynReport(FILE *, int) {
    // asynReport might be called outside of the port thread, so we
    // cannot really query anything which might depend on the internal state,
    // which basically renders this method useless.
}

namespace {

/*
 * Resolve a hostname into an IP address.
 */
std::string resolveHostname(std::string hostname) {
    int status;
    struct addrinfo hints;
    // Ensure addrinfo structure is initialized with zeros.
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags = AI_NUMERICSERV;
    // Only allow IPv4. An IPv6 address would result in a long string that
    // cannot be handled by Snap7 at the moment.
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *firstAddrinfo;
    status = getaddrinfo(hostname.c_str(), nullptr, &hints, &firstAddrinfo);
    if (status != 0) {
        printf("Could not get address info for %s: %s\n", hostname.c_str(), gai_strerror(status));
        return std::string();
    }
    // We only use the first address.
    char buffer[INET_ADDRSTRLEN];
    struct sockaddr_in *sa = reinterpret_cast<struct sockaddr_in *>(firstAddrinfo->ai_addr);
    if (!inet_ntop(sa->sin_family, &sa->sin_addr, buffer, sizeof(buffer))) {
        printf("Could not convert address info for %s to IP address string.\n", hostname.c_str());
        buffer[0] = 0;
    }
    // Free addrinfo structure.
    freeaddrinfo(firstAddrinfo);
    firstAddrinfo = nullptr;
    return std::string(buffer);
}

} // anonymous namesapce

asynStatus PortDriver::asynConnect(asynUser *pasynUser) {
    {
        // Lock mutex while trying to connect. This avoids double connection
        // attempts when asynConnect is called twice within a short time.
        epicsGuard<epicsMutex> guard(this->connectMutex);

        if (this->plcPort <= 0 || this->plcPort > 65535) {
            printf("Port number %d is outside valid range (1-65535).\n", this->plcPort);
            return asynError;
        }
        this->client.RemotePort = this->plcPort;
        // We increase the requested PDU length from the default value of 480 to
        // 960. Some PLCs support such a large value and if supported, it will
        // make the communication more efficient. For PLCs that do not support
        // such a large value, the actual PDU size used will be reduced as part
        // of the negotiation process.
        this->client.PDURequest = 960;
        auto ipAddrString = resolveHostname(this->plcHostname);
        int status = this->client.ConnectTo(ipAddrString.c_str(), this->plcRack, this->plcSlot);
        if (status) {
            printf("Could not connect to PLC for port %s: TSnap7MicroClient::ConnectTo() failed with status code %d.\n", this->portName.c_str(), status);
            return asynError;
        }
        this->connected = true;
    }

    // We send the connect notification without holding the lock to avoid
    // a dead-lock situation.
    pasynManager->exceptionConnect(pasynUser);

    return asynSuccess;
}

asynStatus PortDriver::asynDisconnect(asynUser *pasynUser) {
    // Lock mutex while disconnecting. This avoids interference effects between
    // asynConnect and asynDisconnect.
    {
        epicsGuard<epicsMutex> guard(this->connectMutex);
        if (!connected) {
            // Connection was already cleaned up.
            // We return directly in order to avoid calling
            // exceptionDisconnect again.
            return asynSuccess;
        }
        this->client.Disconnect();
        this->connected = false;
    }

    // We send the disconnect notification without holding the lock to avoid
    // a dead-lock situation.
    pasynManager->exceptionDisconnect(pasynUser);

    return asynSuccess;
}

namespace {

/*
 * Converts to the area values Snap7 expects.
 */
int areaToSnap7Area(s7nodavePlcArea area) {
    switch(area) {
    case plcAreaDb:
        return S7AreaDB;
    case plcAreaFlags:
        return S7AreaMK;
    case plcAreaInputs:
        return S7AreaPE;
    case plcAreaOutputs:
        return S7AreaPA;
    case plcAreaTimer:
        return S7AreaTM;
    case plcAreaCounter:
        return S7AreaCT;
    default:
        return -1;
    }
}

/*
 * Checks the trace I/O mask for the ASYN_TRACEIO_DRIVER bit.
 */
bool shouldTraceIO(asynUser *pasynUser) {
    return (pasynTrace->getTraceMask(pasynUser) & ASYN_TRACEIO_DRIVER) != 0;
}

} // anonymous namesapce

asynStatus PortDriver::asynPlcRead(asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int readLength, void *buffer, int *bytesRead) {
    if (!connected) {
        return asynDisconnected;
    }
    int status = this->client.ReadArea(areaToSnap7Area(area), areaNumber, startByte, readLength, S7WLByte, buffer);
    if (status == noError) {
        *bytesRead = readLength;
        // Building a string from the PLC address is relatively expensive, so
        // only do it if needed.
        if (shouldTraceIO(pasynUser)) {
            asynPrintIO(pasynUser, ASYN_TRACEIO_DRIVER, static_cast<char *>(buffer), readLength, "%s PortDriver::asynPlcRead read %d bytes of data from PLC address %s: ", this->portName.c_str(), readLength, PlcAddress::create(area, areaNumber, plcDataSizeByte, startByte, 0)->toString().c_str());
        }
        return asynSuccess;
    } else {
        int isoError = status & errIsoMask;
        if (isoError == errIsoRecvPacket || isoError == errIsoSendPacket || isoError == errIsoShortPacket) {
            // The connection might be broken. We should close the connection,
            // so that asynManager will try to reconnect.
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while reading %d bytes from PLC address %s, disconnecting...\n", this->portName.c_str(), readLength, PlcAddress::create(area, areaNumber, plcDataSizeByte, startByte, 0)->toString().c_str());
            this->asynDisconnect(pasynUser);
            return asynError;
        } else {
            // Other error
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while reading %d bytes from PLC address %s.\n", this->portName.c_str(), readLength, PlcAddress::create(area, areaNumber, plcDataSizeByte, startByte, 0)->toString().c_str());
            return asynError;
        }
    }
}

namespace {

/*
 * Sets the bytesRead values in a chained read item structure to zero.
 */
void setAllBytesReadToZero(asynS7nodaveReadItem *item) {
    while (item) {
        item->bytesRead = 0;
        item = item->next;
    }
}

/*
 * Copies the result of a (multiple item) read request to the buffers of the
 * corresponding read requests.
 */
void processPlcReadResult(asynUser *pasynUser, const char *portName, const TS7DataItem *dataItems, ReadItemList& correspondingItems) {
    int pos = 0;
    for (auto& readItem : correspondingItems) {
        const TS7DataItem *dataItem = dataItems + pos;
        if (dataItem->Result == noError) {
            // Set bytes read. We do not have to copy the buffer because
            // dataItem->pdata already points to readItem->getBuffer().
            int bytesRead = std::min(dataItem->Amount, readItem->getReadBytes());
            readItem->setBytesRead(bytesRead);
            if (shouldTraceIO(pasynUser)) {
                asynPrintIO(pasynUser, ASYN_TRACEIO_DRIVER, static_cast<char *>(readItem->getBuffer()), bytesRead, "%s PortDriver::asynPlcReadMultipleItems read %d bytes of data from PLC address %s: ", portName, bytesRead, PlcAddress::create(readItem->getArea(), readItem->getAreaNumber(), plcDataSizeByte, readItem->getStartByte(), 0)->toString().c_str());
            }
        } else {
            // There is a problem with the response for this item.
            readItem->setErrorStatus(asynError);
        }
        // Increment position counter and iterator
        pos++;
    }
}

/*
 * Sends several read item to the PLC. Takes care of splitting the items into
 * several requests if they are too big.
 */
int sendRequestToPlc(asynUser *pasynUser, const char *portName, TSnap7MicroClient *client, ReadItemList items) {
    // The maxium payload size is the size of a PDU minus the 14 header bytes.
    // These 14 bytes also include the command and item count. In addition to
    // that, 4 bytes have to be reserved for the header of each requested item.
    unsigned int maxSizeOfPayload = client->PDULength - 14;
    // The PLCs can only handle so many items that the request size does not
    // exceed the maximum PDU size. The header (including the read command and
    // item count) occupies 12 bytes. Each read item occupies another 12 bytes.
    int maxNumberOfItemsPerRequest = (client->PDULength - 12) / 12;
    // There also is a strict limit of 20 items per request.
    maxNumberOfItemsPerRequest = std::min(maxNumberOfItemsPerRequest, MaxVars);
    unsigned int itemsLeft = maxNumberOfItemsPerRequest;
    unsigned int bytesLeft = maxSizeOfPayload;
    bool haveSentItemInThisRun = false;
    ReadItemList itemsInPDU;
    int status = noError;
    TS7DataItem dataItems[MaxVars];
    auto i = items.begin();
    while (!items.empty() || !itemsInPDU.empty()) {
        if (itemsLeft <= 0 || bytesLeft < 6 || (i == items.end() && !haveSentItemInThisRun) || items.empty()) {
            // There are two reasons, why this block is executed:
            // 1) There is no space left in the PDU. This might be because the
            //    request would grow too large (itemsLeft == 0) or because the
            //    data in the response would grow too large (bytesLeft < 6).
            //    The limit of 6 bytes is because the smallest item (one or two
            //    bytes read) occupies 6 bytes.
            // 2) We already went through the list once without sending an item.
            //    Probably all remaining items are too big to be included in the
            //    current PDU. We have to send the PDU now, in order to avoid an
            //    infinite loop.
            // 3) We have processed all items. In this case we have to send the
            //    last request.
            status = client->ReadMultiVars(dataItems, itemsInPDU.size());
            if (status == noError) {
                processPlcReadResult(pasynUser, portName, dataItems, itemsInPDU);
            } else {
                // Make sure the bytes read property of the remaining items is
                // set to zero.
                items.splice(items.end(), itemsInPDU);
                for (auto& readItem : items) {
                    readItem->setBytesRead(0);
                }
                int isoError = status & errIsoMask;
                if (isoError == errIsoRecvPacket || isoError == errIsoSendPacket || isoError == errIsoShortPacket) {
                    // Connection might be broken. We should close the
                    // connection, so that asynManager will try to reconnect.
                    // We cannot disconnect here, but the calling routing will
                    // disconnect, when it sees the status code.
                    asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while reading from PLC, disconnecting...\n", portName);
                } else {
                    asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while reading from PLC.\n", portName);
                }
                return status;
            }
            bytesLeft = maxSizeOfPayload;
            itemsLeft = maxNumberOfItemsPerRequest;
            itemsInPDU.clear();
            haveSentItemInThisRun = true;
            if (items.empty()) {
                // We have sent the last PDU, so we can return.
                return status;
            }
        }
        if (i == items.end()) {
            // We have reached the end of the list, but it is not empty.
            // Therefore we start from the beginning of the remaining list.
            // We have to set haveSentItemInThisRun to false in order to avoid
            // an infinite loop.
            haveSentItemInThisRun = false;
            i = items.begin();
        }
        int readBytes = (*i)->getReadBytes();
        // When reading an odd number of bytes, the PLC adds a padding byte
        // before the next item. This means that when reading an odd number of
        // bytes, we actually need space for one byte more. Such a padding byte
        // is not added for the last item in the list of items, but typically
        // this will not help because it would only make a difference if
        // the maximum PDU size was odd. For this reason, we act like any item
        // with an odd number of bytes (even the last item in the list) needs
        // space for an extra byte.
        int effectiveReadBytes = (readBytes % 2 == 0) ? readBytes : (readBytes + 1);
        // We also have to include the extra four bytes that the header
        // associated with each item occupies in the response.
        effectiveReadBytes += 4;
        if (readBytes == 0) {
            // If there are no bytes to be read, we can simply skip the
            // item.
            (*i)->setBytesRead(0);
            i = items.erase(i);
        } else if (static_cast<unsigned int>(effectiveReadBytes) <= bytesLeft) {
            // The item fits in the PDU.
            TS7DataItem *dataItem = dataItems + itemsInPDU.size();
            dataItem->Area = areaToSnap7Area((*i)->getArea());
            dataItem->DBNumber = (*i)->getAreaNumber();
            dataItem->Start = (*i)->getStartByte();
            dataItem->WordLen = S7WLByte;
            dataItem->Amount = (*i)->getReadBytes();
            dataItem->pdata = (*i)->getBuffer();
            bytesLeft -= effectiveReadBytes;
            itemsLeft--;
            // Move item to list itemsInPDU and proceed with next one
            itemsInPDU.splice(itemsInPDU.end(), items, i++);
        } else {
            // Item is to big for the remaining space in the PDU. We can
            // continue to look for an item that is smaller. However, if the
            // PDU is still empty, the single item is just to big for a single
            // PDU. In this case we use ReadArea, which will automatically split
            // the request.
            if (bytesLeft == maxSizeOfPayload) {
                status = client->ReadArea(areaToSnap7Area((*i)->getArea()), (*i)->getAreaNumber(), (*i)->getStartByte(), (*i)->getReadBytes(), S7WLByte, (*i)->getBuffer());
                if (status != noError) {
                    return status;
                }
                // Remove item from list and continue with next item
                i = items.erase(i);
                // This branch of the if statement is only used when no items
                // have been added to the PDU, so we do not have to reset
                // bytesLeft, itemsLeft, and itemsInPDU.
            } else {
                // We try the next item.
                i++;
            }
        }
    }
    return status;
}

} // anonymous namesapce

asynStatus PortDriver::asynPlcReadMultipleItems(asynUser *pasynUser, asynS7nodaveReadItem *firstItem) {
    if (!this->connected) {
        asynS7nodaveReadItem *nextItem = firstItem;
        while (nextItem) {
            setAllBytesReadToZero(nextItem);
            nextItem = nextItem->next;
        }
        return asynDisconnected;
    }

    // The maxium payload size is the size of a PDU minus the 14 header bytes.
    // In addition to that, 4 bytes have to be reserved for the header of each
    // requested item.
    unsigned int maxSizeOfPayload = this->client.PDULength - 14;

    // Optimize the number of read requests
    ReadItemList readItems = PortDriverReadOptimizer::createOptimizedList(firstItem, maxSizeOfPayload);

    // Send requests to PLC and process responses.
    int status = sendRequestToPlc(pasynUser, this->portName.c_str(), &this->client, readItems);
    int isoError = status & errIsoMask;
    if (isoError == errIsoRecvPacket || isoError == errIsoSendPacket || isoError == errIsoShortPacket) {
        this->asynDisconnect(pasynUser);
    }

    // Dispatch the results from the joined requests to the original individual
    // requests and free the allocated memory. This method has to be called,
    // even if there was an error, in order to avoid a memory leak.
    PortDriverReadOptimizer::dispatchResults(readItems);

    // Check status code
    if (status == noError) {
        return asynSuccess;
    } else {
        return asynError;
    }
}

asynStatus PortDriver::asynPlcWrite(asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int writeLength, void *buffer, int *bytesWritten) {
    if (!this->connected) {
        return asynDisconnected;
    }
    int status = this->client.WriteArea(areaToSnap7Area(area), areaNumber, startByte, writeLength, S7WLByte, buffer);
    if (status == noError) {
        *bytesWritten = writeLength;
        if (shouldTraceIO(pasynUser)) {
            asynPrintIO(pasynUser, ASYN_TRACEIO_DRIVER, static_cast<char *>(buffer), writeLength, "%s PortDriver::asynPlcRead write %d bytes of data to PLC address %s: ", this->portName.c_str(), writeLength, PlcAddress::create(area, areaNumber, plcDataSizeByte, startByte, 0)->toString().c_str());
        }
        return asynSuccess;
    } else {
        int isoError = status & errIsoMask;
        if (isoError == errIsoRecvPacket || isoError == errIsoSendPacket || isoError == errIsoShortPacket) {
            // The connection might be broken. We should close the connection,
            // so that asynManager will try to reconnect.
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while writing %d bytes to PLC address %s, disconnecting...\n", this->portName.c_str(), writeLength, PlcAddress::create(area, areaNumber, plcDataSizeByte, startByte, 0)->toString().c_str());
            this->asynDisconnect(pasynUser);
            return asynError;
        } else {
            // Other error
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while writing %d bytes to PLC address %s.\n", this->portName.c_str(), writeLength, PlcAddress::create(area, areaNumber, plcDataSizeByte, startByte, 0)->toString().c_str());
            return asynError;
        }
    }
}

asynStatus PortDriver::asynPlcWriteBit(asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int startBit, unsigned char bitValue) {
    if (!this->connected) {
        return asynDisconnected;
    }
    bitValue = bitValue ? 1 : 0;
    int status = this->client.WriteArea(areaToSnap7Area(area), areaNumber, startByte * 8 + startBit, 1, S7WLBit, &bitValue);
    if (status == noError) {
        if (shouldTraceIO(pasynUser)) {
            asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s wrote bit %d to PLC address %s.\n", this->portName.c_str(), (bitValue) ? 1 : 0, PlcAddress::create(area, areaNumber, plcDataSizeBit, startByte, startBit)->toString().c_str());
        }
        return asynSuccess;
    } else {
        int isoError = status & errIsoMask;
        if (isoError == errIsoRecvPacket || isoError == errIsoSendPacket || isoError == errIsoShortPacket) {
            // The connection might be broken. We should close the connection,
            // so that asynManager will try to reconnect.
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while writing bit %d to PLC address %s, disconnecting...\n", this->portName.c_str(), (bitValue) ? 1 : 0, PlcAddress::create(area, areaNumber, plcDataSizeBit, startByte, startBit)->toString().c_str());
            this->asynDisconnect(pasynUser);
            return asynError;
        } else {
            // Other error
            asynPrint(pasynUser, ASYN_TRACE_ERROR, "%s error while writing bit %d to PLC address %s.\n", this->portName.c_str(), (bitValue) ? 1 : 0, PlcAddress::create(area, areaNumber, plcDataSizeBit, startByte, startBit)->toString().c_str());
            return asynError;
        }
    }
}

void PortDriver::asynReportStatic(void *drvPvt, FILE *fp, int details) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    driver->asynReport(fp, details);
}

asynStatus PortDriver::asynConnectStatic(void *drvPvt, asynUser *pasynUser) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    return driver->asynConnect(pasynUser);
}

asynStatus PortDriver::asynDisconnectStatic(void *drvPvt, asynUser *pasynUser) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    return driver->asynDisconnect(pasynUser);
}

asynStatus PortDriver::asynPlcReadStatic(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int readLength, void *buffer, int *bytesRead) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    return driver->asynPlcRead(pasynUser, area, areaNumber, startByte, readLength, buffer, bytesRead);
}

asynStatus PortDriver::asynPlcReadMultipleItemsStatic(void *drvPvt, asynUser *pasynUser, asynS7nodaveReadItem *firstItem) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    return driver->asynPlcReadMultipleItems(pasynUser, firstItem);
}

asynStatus PortDriver::asynPlcWriteStatic(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int writeLength, void *buffer, int *bytesWritten) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    return driver->asynPlcWrite(pasynUser, area, areaNumber, startByte, writeLength, buffer, bytesWritten);
}

asynStatus PortDriver::asynPlcWriteBitStatic(void *drvPvt, asynUser *pasynUser, s7nodavePlcArea area, int areaNumber, int startByte, int startBit, unsigned char bitValue) {
    PortDriver *driver = static_cast<PortDriver *>(drvPvt);
    return driver->asynPlcWriteBit(pasynUser, area, areaNumber, startByte, startBit, bitValue);
}

} // namespace s7nodave
