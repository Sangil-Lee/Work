#ifndef s7nodave_PollGroup_h
#define s7nodave_PollGroup_h

#ifdef __cplusplus

#include <list>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <tuple>

#include <epicsMutex.h>
#include <epicsTimer.h>

#include "s7nodaveAsyn.h"

#include "Optional.h"
#include "PlcAddress.h"

namespace s7nodave {

class PollGroup;
class PollService;
class PollRequester;

/**
 * Interface that must be implemented by classes that want to register in a
 * poll-group.
 */
class PollRequester {
public:
    /**
     * Called by the poll-group, when the requester should queue its read
     * request(s) using the passed PollService.
     */
    virtual void prepareRequest(PollService& pollService) = 0;

    /**
     * Called by the poll-group for each read request that has been queued
     * by the requester in the prepare request phase. The parameter
     * requestSucceeded signals whether the request was successful. If this
     * flag is true, the buffer has been filled with bufferSize bytes of data
     * read from the device.
     */
    virtual void processResponse(bool requestSucceeded, int bufferSize, void *buffer) = 0;

    /**
     * Destructor. Explicitly declared as virtual to avoid compiler warnings.
     */
    virtual ~PollRequester() {
    };
};

/**
 * Provides a service to poll requesters for queuing read requests.
 */
class PollService {
    friend class PollGroup;

public:
    /**
     * Queue a read request. The poll-group will read bufferSize bytes from
     * the specified PLC address and place them in buffer. Buffer has to
     * accommodate for at least bufferSize bytes.
     */
    void requestRead(PlcAddress plcAddress, int bufferSize, void *buffer);

private:
    /**
     * Private constructor. The constructor is only used by the friend class
     * PollGroup.
     */
    PollService() {
    };

    /**
     * Private copy constructor. The copy constructor is declared to be private
     * in order to avoid copies of the poll service to be created.
     */
    PollService(const PollService&);

    /**
     * Private assignment operator. The assignment operator is declared to be
     * private in order to avoid copies of the poll service to be created.
     */
    const PollService& operator=(const PollService&);

    /**
     * Stores the queued read requests.
     */
    std::list<std::tuple<PlcAddress, int, void *>> requests;
};

/**
 * Provides polling services for a device. A poll-group will periodically read
 * a set of values from a hardware device. This is more efficient than reading
 * each item individually, because the number of network round-trips is reduced.
 */
class PollGroup {
public:
    /**
     * Destructor. Destroys the timer and timer thread stored within this class.
     */
    virtual ~PollGroup();

    /**
     * Called by a requester to register itself with the poll-group. Once a
     * requester has registered itself, its prepareRequest method will be
     * called each time this poll group is processed.
     */
    void registerRequester(PollRequester *requester);

    /**
     * Called by a registered requester to unregister from the poll-group. Due
     * to the asynchronous nature of the poll-group, the requesters
     * prepareRequest method might be called one more time after it has
     * unregistered.
     */
    void unregisterRequester(PollRequester *requester);

    /**
     * Creates a poll group with the specified name for the specified port.
     * If a poll group with the same name already exists for the port, an error
     * message is displayed and the poll group is not created.
     * The polling interval is specified in seconds.
     * The specified priority is used when creating the thread that periodically
     * processes the poll group. If a priority of zero is specified,
     * epicsThreadPriorityMedium is used.
     * After all poll-groups have been created, they have to be started by
     * calling startPollGroups once.
     */
    static void create(std::string portName, std::string pollGroupName, double pollingInterval, unsigned int priority);

    /**
     * Find a poll group with the specified name for the specified port. Returns
     * either the found poll group or a null pointer, if no poll group with the
     * specified name was found for the specified port.
     */
    static std::shared_ptr<PollGroup> find(std::string portName, std::string pollGroupName);

    /**
     * Starts all registered poll groups for all ports.
     */
    static void startPollGroups();

private:
    /**
     * Class needed to register with the timer queues timer.
     */
    class TimerNotify : public epicsTimerNotify {
    public:
        TimerNotify(PollGroup *pollGroup) : pollGroup(pollGroup) {
        };

        virtual ~TimerNotify() {
        };

        virtual expireStatus expire(const epicsTime& currentTime) override;

        PollGroup *pollGroup;
    };

    /**
     * Private constructor. The static create method should be used to create
     * a poll group.
     */
    PollGroup(std::string portName, std::string pollGroupName, double pollingInterval, unsigned int priority);

    /**
     * Initializes asyn (creates the asynUser, and interfaces, connects the user
     * to the device, etc.).
     */
    asynStatus initAsyn();

    /**
     * Called by the timer each time the poll group should be processed.
     */
    void process();

    /**
     * Name of the poll group.
     */
    std::string name;

    /**
     * Name of the port this poll-group was instantiated for.
     */
    std::string portName;

    /**
     * Polling interval in seconds.
     */
    double pollingInterval;

    /**
     * Mutex used to lock when accessing the list of requesters.
     */
    epicsMutex instanceMutex;

    /**
     * List of registered requesters.
     */
    std::set<PollRequester *> requesters;

    /**
     * Timer queue used for periodically calling process.
     */
    epicsTimerQueueActive& timerQueue;

    /**
     * Timer used for periodically calling process.
     */
    epicsTimer& timer;

    /**
     * Timer notify object used for periodically calling process.
     */
    TimerNotify timerNotify;


    /**
     * asyn user. Needed to access device.
     */
    asynUser *myAsynUser;

    /**
     * Interface holder for s7nodave-specific driver.
     */
    asynInterface *myAsynS7nodaveInterface;

    /**
     * Interface of s7nodave-specific driver.
     */
    asynS7nodave *myAsynS7nodave;

    /**
     * Stores the poll-groups.
     */
    static std::map<std::pair<std::string, std::string>, std::shared_ptr<PollGroup>> pollGroups;

    /**
     * Mutex locked when accessing the map of poll-groups and checking or setting
     * isStarted.
     */
    static epicsMutex staticMutex;

    /**
     * Set to true after startPollGroups has been called.
     */
    static bool isStarted;
};

} // namespace s7nodave

extern "C" {
#endif
/**
 * Called by iocsh support when s7nodaveConfigurePollGroup command is executed.
 */
void s7nodaveConfigurePollGroup(char *portName, char *pollGroupName, double pollingInterval, int priority);

/**
 * Called by initialization hook after IOC has been initialized.
 */
void s7nodaveStartPollGroups();
#ifdef __cplusplus
} // extern "C"
#endif

#endif // s7nodave_PollGroup_h
