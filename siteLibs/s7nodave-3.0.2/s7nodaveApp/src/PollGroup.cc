#include <cmath>

#include <stdio.h>

#include <epicsGuard.h>

#include "PollGroup.h"

namespace s7nodave {

using PollRequest = std::tuple<PlcAddress, int, void *>;
using PollRequestList = std::list<PollRequest>;
using PollRequestWithRequester = std::tuple<PlcAddress, int, void *, PollRequester *>;
using PollRequestWithRequesterList = std::list<PollRequestWithRequester>;
using PollRequesterSet = std::set<PollRequester *>;
using PollGroupMap = std::map<std::pair<std::string, std::string>, std::shared_ptr<PollGroup>>;
using PollGroupEntry = std::pair<std::pair<std::string, std::string>, std::shared_ptr<PollGroup>>;

void PollService::requestRead(PlcAddress plcAddress, int bufferSize, void *buffer) {
    PollRequest request(plcAddress, bufferSize, buffer);
    this->requests.push_back(request);
}

void PollGroup::registerRequester(PollRequester *requester) {
    epicsGuard<epicsMutex> guard(this->instanceMutex);
    this->requesters.insert(requester);
}

void PollGroup::unregisterRequester(PollRequester *requester) {
    epicsGuard<epicsMutex> guard(this->instanceMutex);
    auto i = this->requesters.find(requester);
    if (i != this->requesters.end()) {
        this->requesters.erase(i);
    }
}

void PollGroup::create(std::string portName, std::string pollGroupName, double pollingInterval, unsigned int priority) {
    epicsGuard<epicsMutex> guard(staticMutex);
    if (pollGroups.find(std::make_pair(portName, pollGroupName)) != pollGroups.end()) {
        printf("Error: Duplicate definition of poll group \"%s\" for port \"%s\".\n", pollGroupName.c_str(), portName.c_str());
        return;
    }
    std::shared_ptr<PollGroup> pollGroup = std::shared_ptr<PollGroup>(new PollGroup(portName, pollGroupName, pollingInterval, priority));
    if (pollGroup->initAsyn() != asynSuccess) {
        return;
    }
    pollGroups.insert(PollGroupEntry(std::make_pair(portName, pollGroupName), pollGroup));
}

std::shared_ptr<PollGroup> PollGroup::find(std::string portName, std::string pollGroupName) {
    epicsGuard<epicsMutex> guard(staticMutex);
    auto i = pollGroups.find(std::make_pair(portName, pollGroupName));
    if (i == pollGroups.end()) {
        return std::shared_ptr<PollGroup>();
    } else {
        return i->second;
    }
}

void PollGroup::startPollGroups() {
    epicsGuard<epicsMutex> guard(staticMutex);
    if (isStarted) {
        return;
    }
    for (auto& entry : pollGroups) {
        auto& pollGroup = *entry.second;
        pollGroup.timer.start(pollGroup.timerNotify, pollGroup.pollingInterval);
    }
    isStarted = true;
}

epicsTimerNotify::expireStatus PollGroup::TimerNotify::expire(const epicsTime& currentTime) {
    // Save start time
    const epicsTime& startTime = currentTime;

    // Do polling
    this->pollGroup->process();

    // Schedule next run
    epicsTime endTime = epicsTime::getCurrent();
    double timeConsumed = endTime - startTime;
    double delay = this->pollGroup->pollingInterval;
    double timeToWait = delay - timeConsumed;
    if (timeToWait <= 0) {
        timeToWait = std::min(delay, 0.1);
    }
    return expireStatus(restart, timeToWait);
}

PollGroup::PollGroup(std::string portName, std::string pollGroupName, double pollingInterval, unsigned int priority) :
                name(pollGroupName),
                portName(portName),
                pollingInterval(pollingInterval),
                timerQueue(epicsTimerQueueActive::allocate(false, priority)),
                timer(timerQueue.createTimer()),
                timerNotify(TimerNotify(this)),
                myAsynUser(nullptr),
                myAsynS7nodaveInterface(nullptr),
                myAsynS7nodave(nullptr) {
}

PollGroup::~PollGroup() {
    this->timer.destroy();
    this->timerQueue.release();
    if (this->myAsynUser) {
        pasynManager->disconnect(this->myAsynUser);
        pasynManager->freeAsynUser(this->myAsynUser);
        this->myAsynUser = nullptr;
        this->myAsynS7nodaveInterface = nullptr;
        this->myAsynS7nodave = nullptr;
    }
}

asynStatus PollGroup::initAsyn() {
    // createAsynUser always succeeds (otherwise the thread is suspended)
    // We do not pass a call-back function, because we only use asyn
    // synchronously.
    this->myAsynUser = pasynManager->createAsynUser(nullptr, nullptr);
    this->myAsynUser->userPvt = this;

    // Connect asynUser to device
    const int deviceAddress = 0;
    asynStatus status = pasynManager->connectDevice(this->myAsynUser, this->portName.c_str(), deviceAddress);
    if (status != asynSuccess) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "port \"%s\" poll group \"%s\" PollGroup::initAsyn pasynManager->connectDevice %s\n", this->portName.c_str(), this->name.c_str(), this->myAsynUser->errorMessage);
        return asynError;
    }

    // Get device specific interface
    this->myAsynS7nodaveInterface = pasynManager->findInterface(this->myAsynUser, asynS7nodaveType, 1);
    if (!this->myAsynS7nodaveInterface) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "port \"%s\" poll group \"%s\" PollGroup::initAsyn pasynManager->findInterfacce %s\n", this->portName.c_str(), this->name.c_str(), this->myAsynUser->errorMessage);
        pasynManager->disconnect(this->myAsynUser);
        return asynError;
    }
    this->myAsynS7nodave = static_cast<asynS7nodave *>(this->myAsynS7nodaveInterface->pinterface);

    return asynSuccess;
}

void PollGroup::process() {
    PollRequesterSet requesters;
    {
        // Lock mutex while making a copy of the list of requesters.
        epicsGuard<epicsMutex> guard(this->instanceMutex);
        requesters = this->requesters;
    }
    if (requesters.empty()) {
        return;
    }

    PollRequestWithRequesterList requests;
    PollService service;
    for (PollRequester *requester : requesters) {
        // Ask requester to queue read requests.
        requester->prepareRequest(service);
        // Transfer
        for (auto& pollRequest : service.requests) {
            Optional<PlcAddress> plcAddress;
            int bufferSize;
            void *buffer;
            std::tie(plcAddress, bufferSize, buffer) = pollRequest;
            requests.push_back(std::make_tuple(*plcAddress, bufferSize, buffer, requester));
        }
        // Clear list for next requester
        service.requests.clear();
    }

    // Prepare read request
    asynStatus status = asynSuccess;
    asynS7nodaveReadItem *firstItem = nullptr;
    asynS7nodaveReadItem *item = nullptr;
    for (PollRequestWithRequester& requestWithRequester : requests) {
        if (!firstItem) {
            firstItem = static_cast<asynS7nodaveReadItem *>(pasynManager->memMalloc(sizeof(asynS7nodaveReadItem)));
            item = firstItem;
            if (!item) {
                asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "port \"%s\" poll group \"%s\" PollGroup::process Allocation of %lu bytes of memory failed.\n", this->portName.c_str(), this->name.c_str(), static_cast<unsigned long>(sizeof(asynS7nodaveReadItem)));
                status = asynError;
                break;
            }
        } else {
            asynS7nodaveReadItem *newItem = static_cast<asynS7nodaveReadItem *>(pasynManager->memMalloc(sizeof(asynS7nodaveReadItem)));
            if (!newItem) {
                asynPrint(myAsynUser, ASYN_TRACE_ERROR, "port \"%s\" poll group \"%s\" PollGroup::process Allocation of %lu bytes of memory failed.\n", this->portName.c_str(), this->name.c_str(), static_cast<unsigned long>(sizeof(asynS7nodaveReadItem)));
                status = asynError;
                item = firstItem;
                while (item) {
                    asynS7nodaveReadItem *nextItem = item->next;
                    pasynManager->memFree(item, sizeof(asynS7nodaveReadItem));
                    item = nextItem;
                }
                firstItem = nullptr;
                item = nullptr;
                break;
            }
            item->next = newItem;
            item = newItem;
        }
        Optional<PlcAddress> plcAddress;
        int bufferSize;
        void *buffer;
        std::tie(plcAddress, bufferSize, buffer, std::ignore) = requestWithRequester;
        item->area = plcAddress->getArea();
        item->areaNumber = plcAddress->getAreaNumber();
        item->startByte = plcAddress->getStartByte();
        item->readBytes = bufferSize;
        item->buffer = buffer;
        item->bytesRead = 0;
        item->status = asynSuccess;
        item->next = nullptr;
    }

    // Lock port
    bool portLocked = false;
    if (status == asynSuccess) {
        status = pasynManager->lockPort(this->myAsynUser);
        if (status == asynSuccess) {
            portLocked = true;
        }
    }

    // Send read request
    if (status == asynSuccess) {
        status = this->myAsynS7nodave->plcReadMultipleItems(this->myAsynS7nodaveInterface->drvPvt, myAsynUser, firstItem);
    }

    // Unlock port
    if (portLocked) {
        // If unlockPort fails, we just print an error message, because there
        // is no way to recover.
        asynStatus unlockStatus = pasynManager->unlockPort(this->myAsynUser);
        if (unlockStatus != asynSuccess) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "port \"%s\" poll group \"%s\" PollGroup::process pasynManager->unlockPort %s\n", this->portName.c_str(), this->name.c_str(), this->myAsynUser->errorMessage);
        }
    }

    // Free memory reserved for read items and check for each item whether the
    // read operation was successful.
    std::list<bool> success;
    item = firstItem;
    while (item) {
        asynS7nodaveReadItem *nextItem = item->next;
        // A read operation was successful if the overall status indicates
        // success, the status of the item is OK and the number of bytes that
        // was read is equal to the number of bytes that were requested.
        success.push_back(
            status == asynSuccess
            && item->status == asynSuccess
            && item->bytesRead == item->readBytes);
        pasynManager->memFree(item, sizeof(asynS7nodaveReadItem));
        item = nextItem;
    }
    item = nullptr;
    firstItem = nullptr;

    // Process results. This has to be done always, because the requesters need
    // a chance to free the memory they allocated for the buffers.
    PollRequestWithRequesterList::iterator requestsIterator;
    std::list<bool>::iterator successIterator;
    for (requestsIterator = requests.begin(), successIterator = success.begin();
            requestsIterator != requests.end() && successIterator != success.end();
            requestsIterator++, successIterator++) {
        int bufferSize;
        void *buffer;
        PollRequester *requester;
        std::tie(std::ignore, bufferSize, buffer, requester) = *requestsIterator;
        requester->processResponse(*successIterator, bufferSize, buffer);
    }
}

// Initialize static attributes
PollGroupMap PollGroup::pollGroups;
epicsMutex PollGroup::staticMutex;
bool PollGroup::isStarted = false;

} // namespace s7nodave

using namespace s7nodave;

void s7nodaveConfigurePollGroup(char *portName, char *pollGroupName, double pollingInterval, int priority) {
    unsigned long unsignedPriority;
    if (priority > 0) {
        unsignedPriority = priority;
    } else {
        unsignedPriority = epicsThreadPriorityMedium;
    }
    PollGroup::create(portName, pollGroupName, pollingInterval, unsignedPriority);
}

void s7nodaveStartPollGroups() {
    PollGroup::startPollGroups();
}
