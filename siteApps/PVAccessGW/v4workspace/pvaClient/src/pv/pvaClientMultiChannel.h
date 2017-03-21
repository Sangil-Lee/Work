/* pvaClientMultiChannel.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.02
 */
#ifndef PVACLIENTMULTICHANNEL_H
#define PVACLIENTMULTICHANNEL_H

#ifdef epicsExportSharedSymbols
#   define pvaClientMultiChannelEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/ntmultiChannel.h>
#include <pv/createRequest.h>

#ifdef pvaClientMultiChannelEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef pvaClientMultiChannelEpicsExportSharedSymbols
#endif


#include <pv/pvaClient.h>


namespace epics { namespace pvaClient { 


class PvaClientMultiChannel;
typedef std::tr1::shared_ptr<PvaClientMultiChannel> PvaClientMultiChannelPtr;
class PvaClientMultiGetDouble;
typedef std::tr1::shared_ptr<PvaClientMultiGetDouble> PvaClientMultiGetDoublePtr;
class PvaClientMultiPutDouble;
typedef std::tr1::shared_ptr<PvaClientMultiPutDouble> PvaClientMultiPutDoublePtr;
class PvaClientMultiMonitorDouble;
typedef std::tr1::shared_ptr<PvaClientMultiMonitorDouble> PvaClientMultiMonitorDoublePtr;

class PvaClientNTMultiGet;
typedef std::tr1::shared_ptr<PvaClientNTMultiGet> PvaClientNTMultiGetPtr;
class PvaClientNTMultiPut;
typedef std::tr1::shared_ptr<PvaClientNTMultiPut> PvaClientNTMultiPutPtr;
class PvaClientNTMultiMonitor;
typedef std::tr1::shared_ptr<PvaClientNTMultiMonitor> PvaClientNTMultiMonitorPtr;
class PvaClientNTMultiData;
typedef std::tr1::shared_ptr<PvaClientNTMultiData> PvaClientNTMultiDataPtr;


typedef epics::pvData::shared_vector<PvaClientChannelPtr> PvaClientChannelArray;

/**
 * @brief PvaMultiChannel is a synchronous interface for accessing multiple channels.
 *
 * @author mrk
 */
class epicsShareClass PvaClientMultiChannel :
    public std::tr1::enable_shared_from_this<PvaClientMultiChannel>
{
public:
    POINTER_DEFINITIONS(PvaClientMultiChannel);
    /** Create a PvaClientMultiChannel.
     * @param pvaClient The interface to pvaClient.
     * @param channelNames The names of the channel..
     * @param providerName The name of the provider.
     * @param maxNotConnected The maximum number of channels that can be disconnected.
     * @return The interface to the PvaClientMultiChannel
     */
    static PvaClientMultiChannelPtr create(
         PvaClientPtr const &pvaClient,
         epics::pvData::shared_vector<const std::string> const & channelNames,
         std::string const & providerName = "pva",
         size_t maxNotConnected=0
     );
     /**
     * Destructor
     */
    ~PvaClientMultiChannel();
    /** Destroy the pvAccess connections.
     */
    void destroy();
    /** Get the channelNames.
     * @return The names.
     */
    epics::pvData::shared_vector<const std::string> getChannelNames();
    /** Connect to the channels.
     * This calls issueConnect and waitConnect.
     * An exception is thrown if connect fails.
     * @param timeout The time to wait for connecting to the channel.
     * @return status of request
     */
    epics::pvData::Status connect(double timeout=5);
    /** Are all channels connected?
     * @return if all are connected.
     */
    bool allConnected();
    /** Has a connection state change occured?
     * @return (true, false) if (at least one, no) channel has changed state.
     */
    bool connectionChange();
    /** Get the connection state of each channel.
     * @return The state of each channel.
     */
    epics::pvData::shared_vector<epics::pvData::boolean> getIsConnected();
    /** Get the pvaClientChannelArray.
     * @return The shared pointer.
     */
    PvaClientChannelArray getPvaClientChannelArray();
    /** Get pvaClient.
     * @return The shared pointer.
     */
    PvaClientPtr getPvaClient();
    /**
     * create a pvaClientMultiGetDouble
     * @return The interface.
     */
    PvaClientMultiGetDoublePtr createGet();   
    /**
     * create a pvaClientMultiPutDouble
     * @return The interface.
     */
    PvaClientMultiPutDoublePtr createPut();
    /**
     * Create a pvaClientMultiMonitorDouble.
     * @return The interface.
     */
    PvaClientMultiMonitorDoublePtr createMonitor();
    /**
     * Create a pvaClientNTMultiPut.
     * @return The interface.
     */
    PvaClientNTMultiPutPtr createNTPut();
    /**
     * Create a pvaClientNTMultiGet.
     * This calls the next method with request = "value,alarm,timeStamp"
     * @return The interface.
     */
    PvaClientNTMultiGetPtr createNTGet();
    /**
     * Create a pvaClientNTMultiGet;
     * @param request The request for each channel.
     * @return The interface.
     */
    PvaClientNTMultiGetPtr createNTGet(std::string const &request);
    /**
     * Create a pvaClientNTMultiMonitor.
     * This calls the next method with request = "value,alarm,timeStamp"
     * @return The interface.
     */
    PvaClientNTMultiMonitorPtr createNTMonitor();
    /**
     * Create a pvaClientNTMultiPut.
     * @param request The request for each channel.
     * @return The interface.
     */
    PvaClientNTMultiMonitorPtr createNTMonitor(std::string const &request);
    /** Get the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientMultiChannelPtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientMultiChannel(
        PvaClientPtr const &pvaClient,
        epics::pvData::shared_vector<const std::string> const & channelName,
        std::string const & providerName,
        size_t maxNotConnected);

    void checkConnected();
    
    PvaClientPtr pvaClient;
    epics::pvData::shared_vector<const std::string> channelName;
    std::string providerName;
    size_t maxNotConnected;

    size_t numChannel;
    epics::pvData::Mutex mutex;

    size_t numConnected;
    PvaClientChannelArray pvaClientChannelArray;
    epics::pvData::shared_vector<epics::pvData::boolean> isConnected;
    epics::pvData::CreateRequest::shared_pointer createRequest;
    bool isDestroyed;
    
};

/**
 *  @brief Provides channelGet to multiple channels where each channel has a numeric scalar value field.
 */
class epicsShareClass PvaClientMultiGetDouble :
    public std::tr1::enable_shared_from_this<PvaClientMultiGetDouble>
{

public:
    POINTER_DEFINITIONS(PvaClientMultiGetDouble);
    
    /**
     * Factory method that creates a PvaClientMultiGetDouble.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @return The interface.
     */
    static PvaClientMultiGetDoublePtr create(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);

    ~PvaClientMultiGetDouble();

    /** Destroy the pvAccess connection.
     */
    void destroy();
     /**
     * Create a channelGet for each channel.
     */
    void connect();
    /**
     * get the data.
     * @return The double[] where each element is the value field of the corresponding channel.
     */
    epics::pvData::shared_vector<double> get();
    /** Get the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientMultiGetDoublePtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientMultiGetDouble(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    size_t nchannel;
    epics::pvData::Mutex mutex;
    
    epics::pvData::shared_vector<double> doubleValue;
    std::vector<PvaClientGetPtr> pvaClientGet;
    bool isGetConnected;
    bool isDestroyed;
};

/**
 * @brief Provides channelPut to multiple channels where each channel has a numeric scalar value field.
 */
class epicsShareClass PvaClientMultiPutDouble :
    public std::tr1::enable_shared_from_this<PvaClientMultiPutDouble>
{

public:
    POINTER_DEFINITIONS(PvaClientMultiPutDouble);
   
    /**
     * Factory method that creates a PvaClientMultiPutDouble.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @return The interface.
     */
    static PvaClientMultiPutDoublePtr create(
         PvaClientMultiChannelPtr const &pvaMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);
    ~PvaClientMultiPutDouble();

    /** Destroy the pvAccess connection.
     */
    void destroy();
     /**
     * Create a channelPut for each channel.
     */
    void connect();
    /** put data to each channel as a double
     * @param data The array of data for each channel.
     */
    void put(epics::pvData::shared_vector<double> const &data);
    /** Get the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientMultiPutDoublePtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientMultiPutDouble(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    size_t nchannel;
    epics::pvData::Mutex mutex;

    std::vector<PvaClientPutPtr> pvaClientPut;
    bool isPutConnected;
    bool isDestroyed;
};

/**
 * @brief Provides a monitor to multiple channels where each channel has a numeric scalar value field.
 */
class epicsShareClass PvaClientMultiMonitorDouble :
    public std::tr1::enable_shared_from_this<PvaClientMultiMonitorDouble>
{

public:
    POINTER_DEFINITIONS(PvaClientMultiMonitorDouble);
    
    /**
     * Factory method that creates a PvaClientMultiMonitorDouble.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @return The interface.
     */
    static PvaClientMultiMonitorDoublePtr create(
         PvaClientMultiChannelPtr const &pvaMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);
    ~PvaClientMultiMonitorDouble();

    /** Destroy the pvAccess connection.
     */
    void destroy();
     /**
     * Create a channel monitor for each channel.
     */
    void connect();
     /**
     * poll each channel.
     * If any has new data it is used to update the double[].
     * @return (false,true) if (no, at least one) value was updated.
     */
    bool poll();
    /**
     * Wait until poll returns true.
     * @param waitForEvent The time to keep trying.
     * A thread sleep of .1 seconds occurs between each call to poll.
     * @return (false,true) if (timeOut, poll returned true).
     */
    bool waitEvent(double waitForEvent);
    /**
     * get the data.
     *  @return The double[] where each element is the value field of the corresponding channel.
     */
    epics::pvData::shared_vector<double> get();
    /** Monitor the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientMultiMonitorDoublePtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientMultiMonitorDouble(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    size_t nchannel;
    epics::pvData::Mutex mutex;

    epics::pvData::shared_vector<double> doubleValue;
    std::vector<PvaClientMonitorPtr> pvaClientMonitor;
    bool isMonitorConnected;
    bool isDestroyed;
};

/**
 *  @brief Provides channelGet to multiple channels where the value field of each channel is presented as a union.
 */
class epicsShareClass PvaClientNTMultiGet :
    public std::tr1::enable_shared_from_this<PvaClientNTMultiGet>
{

public:
    POINTER_DEFINITIONS(PvaClientNTMultiGet);
     /**
     * Factory method that creates a PvaClientNTMultiGet.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @param pvRequest The pvRequest for each channel.
     * @return The interface.
     */
    static PvaClientNTMultiGetPtr create(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest);

    ~PvaClientNTMultiGet();

    /** Destroy the pvAccess connection.
     */
    void destroy();
     /**
     * Create a channelGet for each channel.
     */
    void connect();
    /**
     * get data for each channel.
     */
    void get();
    /**
     * get the data.
     * @return the pvaClientNTMultiData.
     */
    PvaClientNTMultiDataPtr getData();
    /** Get the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientNTMultiGetPtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientNTMultiGet(
         epics::pvData::UnionConstPtr const & u,
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    epics::pvData::PVStructurePtr pvRequest;
    size_t nchannel;
    epics::pvData::Mutex mutex;
    
    
    PvaClientNTMultiDataPtr pvaClientNTMultiData;
    std::vector<PvaClientGetPtr> pvaClientGet;
    bool isConnected;
    bool isDestroyed;
};

/**
 *  @brief Provides channelPut to multiple channels where the value field of each channel is presented as a union.
 */
class epicsShareClass PvaClientNTMultiPut :
    public std::tr1::enable_shared_from_this<PvaClientNTMultiPut>
{

public:
    POINTER_DEFINITIONS(PvaClientNTMultiPut);
    /**
     * Factory method that creates a PvaClientNTMultiPut.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @return The interface.
     */
    static PvaClientNTMultiPutPtr create(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);
    ~PvaClientNTMultiPut();

    /** Destroy the pvAccess connection.
     */
    void destroy();
     /**
     * Create a channelPut for each channel.
     */
    void connect();
    /**
     * get the value field of each channel as a union.
     * @return A shared vector of union.
     */
    epics::pvData::shared_vector<epics::pvData::PVUnionPtr> getValues();
    /**
     * put the data to each channel.
'    */
    void put();
    /** Get the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientNTMultiPutPtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientNTMultiPut(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    size_t nchannel;
    epics::pvData::Mutex mutex;

    epics::pvData::shared_vector<epics::pvData::PVUnionPtr> unionValue;
    epics::pvData::shared_vector<epics::pvData::PVFieldPtr> value;
    std::vector<PvaClientPutPtr> pvaClientPut;
    bool isConnected;
    bool isDestroyed;
};

/**
 *  @brief Provides channel monitor to multiple channels where the value field of each channel is presented as a union.
 */
class epicsShareClass PvaClientNTMultiMonitor :
    public std::tr1::enable_shared_from_this<PvaClientNTMultiMonitor>
{

public:
    POINTER_DEFINITIONS(PvaClientNTMultiMonitor);
    /**
     * Factory method that creates a PvaClientNTMultiMonitor.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @param pvRequest The pvRequest for each channel.
     * @return The interface.
     */
    static PvaClientNTMultiMonitorPtr create(
         PvaClientMultiChannelPtr const &pvaNTMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest);
    ~PvaClientNTMultiMonitor();

    /** Destroy the pvAccess connection.
     */
    void destroy();
     /**
     * Create a channel monitor for each channel.
     */
    void connect();
     /**
     * poll each channel.
     * If any has new data it is used to update the double[].
     * @return (false,true) if (no, at least one) value was updated.
     */
    bool poll();
    /**
     * Wait until poll returns true.
     * @param waitForEvent The time to keep trying.
     * A thread sleep of .1 seconds occurs between each call to poll.
     * @return (false,true) if (timeOut, poll returned true).
     */
    bool waitEvent(double waitForEvent);
     /**
     * get the data.
     * @return the pvaClientNTMultiData.
     */
    PvaClientNTMultiDataPtr getData();
    /** Monitor the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientNTMultiMonitorPtr getPtrSelf()
    {
        return shared_from_this();
    }
private:
    PvaClientNTMultiMonitor(
         epics::pvData::UnionConstPtr const & u,
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    epics::pvData::PVStructurePtr pvRequest;
    size_t nchannel;
    epics::pvData::Mutex mutex;
    PvaClientNTMultiDataPtr pvaClientNTMultiData;
    std::vector<PvaClientMonitorPtr> pvaClientMonitor;
    bool isConnected;
    bool isDestroyed;
};

/**
 *  @brief Provides NTMultiChannel data for both PvaClientNTMultiGet and PvaClientNTMultiMonitor.
 */
class epicsShareClass PvaClientNTMultiData :
    public std::tr1::enable_shared_from_this<PvaClientNTMultiData>
{

public:
    POINTER_DEFINITIONS(PvaClientNTMultiData);
    /**
     * Factory method that creates a PvaClientNTMultiData.
     * Normally only called by PvaClientNTMultiGet and PvaClientNTMultiMonitor.
     * @param u The union interface for the value field of each channel.
     * @param pvaClientMultiChannel The interface to PvaClientMultiChannel.
     * @param pvaClientChannelArray The PvaClientChannel array.
     * @param pvRequest The pvRequest for each channel.
     */
    static PvaClientNTMultiDataPtr create(
         epics::pvData::UnionConstPtr const & u,
         PvaClientMultiChannelPtr const &pvaNTMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest);
    ~PvaClientNTMultiData();
    /** Destroy the pvAccess connection.
     */
    void destroy();

    /**
     * Get the number of channels.
     * @return The number of channels.
     */
    size_t getNumber();
   
    /**
     * Set the timeStamp base for computing deltaTimes. 
     */
    void startDeltaTime();
   
    /**
     * Update NTMultiChannel fields.
     */
    void endDeltaTime();
    /**
     * Get the time when the last get was made.
     * @return The timeStamp.
     */
    epics::pvData::TimeStamp getTimeStamp(); 
    /**
     * Get the  NTMultiChannel.
     * @return The value.
     */
    epics::nt::NTMultiChannelPtr getNTMultiChannel();
    /** Get the shared pointer to self.
     * @return The shared pointer.
     */
    PvaClientNTMultiDataPtr getPtrSelf()
    {
        return shared_from_this();
    }
   
private:
    PvaClientNTMultiData(
         epics::pvData::UnionConstPtr const & u,
         PvaClientMultiChannelPtr const &pvaNTMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest);
    void setStructure(epics::pvData::StructureConstPtr const & structure,size_t index);
    void setPVStructure(
        epics::pvData::PVStructurePtr const &pvStructure,size_t index);

    PvaClientMultiChannelPtr pvaClientMultiChannel;
    PvaClientChannelArray pvaClientChannelArray;
    size_t nchannel;
    epics::pvData::Mutex mutex;

    std::vector<epics::pvData::PVStructurePtr> topPVStructure;
    bool gotAlarm;
    bool gotTimeStamp;
    bool isDestroyed;
    
    epics::pvData::StructureConstPtr ntMultiChannelStructure;
    epics::pvData::shared_vector<epics::pvData::PVUnionPtr> unionValue;
    epics::pvData::shared_vector<epics::pvData::int32> severity;
    epics::pvData::shared_vector<epics::pvData::int32> status;
    epics::pvData::shared_vector<std::string> message;
    epics::pvData::shared_vector<epics::pvData::int64> secondsPastEpoch;
    epics::pvData::shared_vector<epics::pvData::int32> nanoseconds;
    epics::pvData::shared_vector<epics::pvData::int32> userTag;
    epics::pvData::Alarm alarm;
    epics::pvData::TimeStamp timeStamp;;
    epics::pvData::PVTimeStamp pvTimeStamp;
    friend class PvaClientNTMultiGet;
    friend class PvaClientNTMultiPut;
    friend class PvaClientNTMultiMonitor;
};


}}

#endif  /* PVACLIENTMULTICHANNEL_H */

