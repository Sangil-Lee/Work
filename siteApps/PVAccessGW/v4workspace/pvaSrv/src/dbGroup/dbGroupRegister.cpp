/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>

#include <iocsh.h>
#include <epicsExport.h>
#include <epicsThread.h>

#include <pv/channelProviderLocal.h>

#include "dbGroup.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvIOC;
using namespace epics::pvaSrv;
using std::tr1::static_pointer_cast;


static string requesterName("dbGroupCreate");

class DbGroupConfig;
typedef std::tr1::shared_ptr<DbGroupConfig> DbGroupConfigPtr;

class DbGroupConfig : public Requester
{
public:
    POINTER_DEFINITIONS(DbGroupConfig);
    std::vector<DbGroupProviderPtr> providerArray;
    virtual ~DbGroupConfig() {}
    virtual string getRequesterName() {return requesterName;}
    virtual void message(String const & message,MessageType messageType)
    {
         printf("%s %s\n",requesterName.c_str(),message.c_str());
    }
};

static DbGroupConfigPtr dbGroupConfig(new DbGroupConfig());

static const iocshArg dbGroupCreateArg0 = {"configFileName", iocshArgString};
static const iocshArg *dbGroupCreateArgs[] = {&dbGroupCreateArg0};
static const iocshFuncDef dbGroupCreateFuncDef =
  {"dbGroupCreate", 1, dbGroupCreateArgs};

extern "C" void dbGroupCreate(const iocshArgBuf *args)
{
    string fileName(args[0].sval);
    epicsThreadSleep(.1);
    FILE *f;
    f = fopen(fileName.c_str(),"r");
    if(f==NULL) {
        printf("dbGroupCreate file %s open failure\n",fileName.c_str());
        return;
    }
    string valueProvider("dbPv");
    string channelName;
    StringArrayPtr fieldNames(new StringArray());
    StringArrayPtr valueChannelNames(new StringArray());
    char buffer[180];
    bool gotStart = false;
    while(true) {
        char *result = fgets(buffer,180,f);
        if(result==NULL) break;
        string line(buffer);
        size_t pos = line.find("channelValueProvider");
        if(pos!=std::string::npos) {
              pos = line.find(' ');
              line = line.substr(pos+1);
              pos = line.find('\n');
              line = line.substr(0,pos);
              valueProvider = line;
              continue;
        } 
        pos = line.find("channelName");
        if(pos!=std::string::npos) {
              pos = line.find(' ');
              line = line.substr(pos+1);
              pos = line.find('\n');
              line = line.substr(0,pos);
              channelName = line;
              continue;
        } 
        pos = line.find("channelValue");
        if(pos!=std::string::npos) gotStart = true;
        break;
    }
    if(!gotStart) {
         printf("dbGroupCreate fileName %s bad syntax\n",
            fileName.c_str());
         fclose(f);
         return;
    }
    fpos_t fpos;
    fgetpos(f,&fpos);
    int nlines = 0;
    while(true) {
        char *result = fgets(buffer,180,f);
        if(result==NULL) break;
        nlines++;
    }
    fieldNames->reserve(nlines);
    valueChannelNames->reserve(nlines);
    fsetpos(f,&fpos);
    bool isOK = true;
    for(int i=0; i<nlines; i++) {
        char *result = fgets(buffer,180,f);
        if(result==NULL) {
            printf("dbGroupCreate fileName %s bad syntax\n",
               fileName.c_str());
            fclose(f);
            return;
        }
        string line(buffer);
        size_t pos = line.find_first_not_of(' ');
        line = line.substr(pos);
        pos = line.find_first_of(' ');
        string fieldName = line.substr(0,pos);
        fieldNames->push_back(fieldName);
        line = line.substr(pos+1);
        pos = line.find_first_not_of(' ');
        line = line.substr(pos);
        pos = line.find_first_of('\n');
        string valueChannelName = line.substr(0,pos);
        valueChannelNames->push_back(valueChannelName);
    }
    fclose(f);
    if(!isOK) {
        printf("dbGroupCreate fileName %s bad syntax for channelValues\n",
           fileName.c_str());
        return;
    }
    ChannelAccess::shared_pointer channelAccess = getChannelAccess();
    ChannelProvider::shared_pointer channelProvider =
        channelAccess->getProvider(valueProvider);
    if(channelProvider.get()==NULL) {
        printf("channelProvider %s not found\n", valueProvider.c_str());
        return;
    }
    DbGroupProviderPtr provider(
        new DbGroupProvider(
             dbGroupConfig,
             channelProvider,
             channelName,
             fieldNames,
             valueChannelNames));
    size_t oldSize = dbGroupConfig->providerArray.size();
    dbGroupConfig->providerArray.reserve(oldSize+1);
    dbGroupConfig->providerArray.push_back(provider);
    channelProvider = channelAccess->getProvider("local");
    if(channelProvider.get()==NULL) {
        printf("channelProvider local not found\n");
        return;
    }
    ChannelProviderLocalPtr channelProviderLocal =
         static_pointer_cast<ChannelProviderLocal>(channelProvider);
    channelProviderLocal->registerProvider(channelName,provider);
}

static void dbGroupRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&dbGroupCreateFuncDef, dbGroupCreate);
    }
}

extern "C" {
    epicsExportRegistrar(dbGroupRegister);
}
