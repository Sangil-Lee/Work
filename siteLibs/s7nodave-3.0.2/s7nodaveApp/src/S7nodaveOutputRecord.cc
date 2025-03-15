#include <stdlib.h>
#include <string.h>

#include <alarm.h>
#include <callback.h>
#include <epicsGuard.h>
#include <recGbl.h>

#include "s7nodave.h"

#include "S7nodaveOutputRecord.h"

namespace s7nodave {

long S7nodaveOutputRecord::initRecord() {
    // Most of the initialization is done in the superclass's method.
    long initStatus = S7nodaveRecord::initRecord();
    if (initStatus != RECORD_STATUS_OK) {
        // initRecord failed
        return initStatus;
    }

    // Give child classes a chance to run some code of their own before
    // the initial value of the record is read from hardware.
    initStatus = interceptInitRecordBeforeValueInit();
    if (initStatus != RECORD_STATUS_OK) {
        // initRecord failed in child class
        return initStatus;
    }

    // For an output record, we want to read the current value from
    // the device once and initialize the record with it.
    // We do this using a synchronous read request. Therefore we have
    // to lock the port.
    asynStatus status = pasynManager->lockPort(this->myAsynUser);
    if (status != asynSuccess) {
        // Getting the current value is not crucial for the record
        // initialization, so we just return.
        return RECORD_STATUS_OK;
    }
    bool success = true;
    int bufferSize = this->getIoBufferSizeInBytes();
    void *buffer = pasynManager->memMalloc(bufferSize);
    if (!buffer) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveOutputRecord::initRecord Allocation of %d bytes of memory failed.\n", record->name, bufferSize);
        success = false;
    }
    if (success) {
        success = this->readFromPlc(buffer, bufferSize);
    }
    long recordStatus;
    if (success) {
        recordStatus = this->writeToRecord(buffer, bufferSize);
        this->record->udf = 0;
        recGblResetAlarms(this->record);
    } else {
        recordStatus = RECORD_STATUS_OK;
    }
    if (buffer) {
        pasynManager->memFree(buffer, bufferSize);
        buffer = nullptr;
    }
    // We do not check the status of unlockPort, because we could not do anything
    // about an error anyway.
    pasynManager->unlockPort(this->myAsynUser);
    return recordStatus;
}

void S7nodaveOutputRecord::asynProcessCallback() {
    size_t bufferSize;
    void *buffer;
    // Lock mutex to avoid race condition with processRecord
    {
        epicsGuard<epicsMutex> guard(this->newDataMutex);
        bufferSize = this->newDataLength;
        buffer = this->newData;
        this->newDataLength = 0;
        this->newData = nullptr;
    }
    bool success = true;
    if (!buffer) {
        asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveOutputRecord::asynProcessCallback Got NULL pointer instead of buffer.\n", record->name);
        success = false;
    }
    if (success) {
        success = this->writeToPlc(buffer, bufferSize);
    }
    // Free buffer
    if (buffer) {
        pasynManager->memFree(buffer, bufferSize);
        buffer = nullptr;
    }
    this->processCallbackError = !success;
    if (!success) {
        recGblSetSevr(this->record, WRITE_ALARM, INVALID_ALARM);
    }
    // Although it is not obvious, we can check record->pact here without having
    // to lock a mutex. A mutex in asynManager ensures, that the thread
    // executing this code will see a change of record->pact performed by the
    // thread executing processRecord().
    if (this->record->pact) {
        asynPrint(this->myAsynUser, ASYN_TRACE_FLOW, "%s S7nodaveOutputRecord::asynProcessCallback queuing request for process callback to be run again.\n", this->record->name);
        callbackRequestProcessCallback(&this->processCallback, this->record->prio, this->record);
    }
}

long S7nodaveOutputRecord::processRecord() {
    if (this->record->pact == 0) {
        // Record is not process at the moment, so a write process should be
        // started.
        int bufferSize = this->getIoBufferSizeInBytes();
        void *buffer = pasynManager->memMalloc(bufferSize);
        if (!buffer) {
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveOutputRecord::processRecord Allocation of %d bytes of memory failed.\n", record->name, bufferSize);
            recGblSetSevr(this->record, WRITE_ALARM, INVALID_ALARM);
            return RECORD_STATUS_OK;
        }
        this->readFromRecord(buffer, bufferSize);

        // We lock the mutex in order to avoid a race condition
        // with asynProcessCallback
        {
            epicsGuard<epicsMutex> guard(this->newDataMutex);
            // Make sure that old buffer is freed if not null.
            if (this->newData) {
                pasynManager->memFree(this->newData, this->newDataLength);
            }
            this->newData = buffer;
            this->newDataLength = bufferSize;
        }
        // We have to set PACT in order to signal that the record is being
        // processed asynchronously and to avoid processRecord being called
        // again before the asynchronous write has finished.
        this->record->pact = 1;
        asynStatus status = pasynManager->queueRequest(this->myAsynUser, asynQueuePriorityMedium, 0);
        if (status == asynSuccess) {
            return RECORD_STATUS_OK;
        } else {
            // Reset PACT so that processRecord might be called again later.
            this->record->pact = 0;
            asynPrint(this->myAsynUser, ASYN_TRACE_ERROR, "%s S7nodaveOutputRecord::processRecord queueRequest %s\n", record->name, this->myAsynUser->errorMessage);
            recGblSetSevr(this->record, WRITE_ALARM, INVALID_ALARM);
        }
    } else {
        // Reset UDF flag if last write was successful
        if (!this->processCallbackError) {
            this->record->udf = 0;
        }
    }
    return RECORD_STATUS_OK;
}

long S7nodaveOutputRecord::interceptInitRecordBeforeValueInit() {
    return RECORD_STATUS_OK;
}

} // namespace s7nodave
