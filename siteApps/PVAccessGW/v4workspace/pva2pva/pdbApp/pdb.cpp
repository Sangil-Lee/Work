
#include <vector>
#include <utility>

#include <errlog.h>
#include <epicsString.h>

#include "helper.h"
#include "pdbsingle.h"
#include "pdbgroup.h"
#include "pvif.h"

namespace pvd = epics::pvData;
namespace pva = epics::pvAccess;

int PDBProviderDebug = 3;

namespace {

struct Splitter {
    const char sep, *cur, *end;
    Splitter(const char *s, char sep)
        :sep(sep), cur(s)
    {
        assert(s);
        end = strchr(cur, sep);
    }
    bool operator!() const { return !cur; }
    bool snip(std::string& ret) {
        if(!cur) return false;
        if(end) ret = std::string(cur, end-cur);
        else    ret = std::string(cur);
        if(end) {
            cur = end+1;
            end = strchr(cur, sep);
        } else {
            cur = NULL;
        }
        return true;
    }
};

struct GroupMemberInfo {
    GroupMemberInfo(const std::string& a, const std::string& b) :pvname(a), pvfldname(b) {}
    std::string pvname, pvfldname;
    std::set<size_t> triggers; // indices in GroupInfo::members which are post()d on events from pvfldname
    size_t index; // index in GroupInfo::members
};

struct GroupInfo {
    GroupInfo(const std::string& name) : name(name),atomic(Unset),hastriggers(false) {}
    std::string name;
    typedef std::vector<GroupMemberInfo> members_t;
    members_t members;
    typedef std::map<std::string, size_t> members_map_t;
    members_map_t members_map;
    typedef std::set<std::string> triggers_set_t;
    typedef std::map<std::string, triggers_set_t> triggers_t;
    triggers_t triggers;
    enum tribool {Unset,True,False} atomic;
    bool hastriggers;
};

// Iterates all PDB records and gathers info() to construct PDB groups
struct PDBProcessor
{
    typedef std::map<std::string, GroupInfo> groups_t;
    groups_t groups;

    std::string recbase;
    GroupInfo *curgroup;

    // split off the group name from an info() value
    // find or create this group
    void setupGroup(const char **pvalue)
    {
        const char *start = *pvalue,
                   *split = strchr(start, '|');
        if(!split) {
            std::ostringstream strm;
            strm<<"Expected '|' in \""<<start<<"\"";
            throw std::runtime_error(strm.str());
        }else if(start==split) {
            std::ostringstream strm;
            strm<<"Empty group name in \""<<start<<"\"";
            throw std::runtime_error(strm.str());
        }

        std::string groupname(start, split-start);

        groups_t::iterator it = groups.find(groupname);
        if(it==groups.end()) {
            // lazy creation of group
            std::pair<groups_t::iterator, bool> ins(groups.insert(std::make_pair(groupname, GroupInfo(groupname))));
            it = ins.first;
        }
        curgroup = &it->second;

        *pvalue = split+1; // write back first char after '|'
    }

    // process "pdbGroup" to create/extend PDB to PVA mappings
    void addMappings(const char *value)
    {
        Splitter tok(value, '|');
        std::string mapent;

        while(tok.snip(mapent))
        {
            size_t eq = mapent.find_first_of('=');
            if(eq==mapent.npos) {
                std::ostringstream strm;
                strm<<"Expected '=' in \""<<value<<"\"";
                throw std::runtime_error(strm.str());
            }

            std::string pvf(mapent.substr(0, eq)),
                        dbf(mapent.substr(eq+1));

            if(pvf.empty() || dbf.empty()) {
                std::ostringstream strm;
                strm<<"empty PVA or DB field name in \""<<value<<"\"";
                throw std::runtime_error(strm.str());
            }

            curgroup->members.push_back(GroupMemberInfo(recbase + dbf, pvf));
            curgroup->members.back().index = curgroup->members.size()-1;
            curgroup->members_map[pvf] = curgroup->members.back().index;

            if(PDBProviderDebug>2) {
                fprintf(stderr, "  pdb map '%s.%s' <-> '%s'\n",
                        curgroup->name.c_str(),
                        curgroup->members.back().pvfldname.c_str(),
                        curgroup->members.back().pvname.c_str());
            }
        }
    }

    // process "pdbTrigger" to create/extend PDB to PVA monitor trigger mappings
    void addTriggers(const char *value)
    {
        Splitter tok(value, '|');
        std::string trigent;

        while(tok.snip(trigent))
        {
            size_t eq = trigent.find_first_of('>');
            if(eq==trigent.npos) {
                std::ostringstream strm;
                strm<<"Expected '>' in \""<<value<<"\"";
                throw std::runtime_error(strm.str());
            }

            std::string pvf(trigent.substr(0, eq)),
                        trigs(trigent.substr(eq+1));

            GroupInfo::triggers_t::iterator it = curgroup->triggers.find(pvf);
            if(it==curgroup->triggers.end()) {
                std::pair<GroupInfo::triggers_t::iterator, bool> ins(curgroup->triggers.insert(
                                                                         std::make_pair(pvf, GroupInfo::triggers_set_t())));
                it = ins.first;
            }

            Splitter sep(trigs.c_str(), ',');
            std::string target;

            while(sep.snip(target)) {
                curgroup->hastriggers = true;
                it->second.insert(target);
            }
        }
    }

    // validate trigger mappings and process into bit map form
    void resolveTriggers()
    {
        FOREACH(it, end, groups) { // for each group
            GroupInfo& info = it->second;

            if(info.hastriggers) {
                FOREACH(it2, end2, info.triggers) { // for each trigger source
                    const std::string& src = it2->first;
                    GroupInfo::triggers_set_t& targets = it2->second;

                    GroupInfo::members_map_t::iterator it2x = info.members_map.find(src);
                    if(it2x==info.members_map.end()) {
                        fprintf(stderr, "Group \"%s\" defines triggers from non-existant field \"%s\"\n",
                                info.name.c_str(), src.c_str());
                        continue;
                    }
                    GroupMemberInfo& srcmem = info.members[it2x->second];

                    if(PDBProviderDebug>2)
                        fprintf(stderr, "  pdb trg '%s.%s'  -> ",
                                info.name.c_str(), src.c_str());

                    FOREACH(it3, end3, targets) { // for each trigger target
                        const std::string& target = *it3;

                        if(target=="*") {
                            for(size_t i=0; i<info.members.size(); i++) {
                                srcmem.triggers.insert(i);
                                if(PDBProviderDebug>2)
                                    fprintf(stderr, "%s, ", info.members[i].pvfldname.c_str());
                            }

                        } else {

                            GroupInfo::members_map_t::iterator it3x = info.members_map.find(target);
                            if(it3x==info.members_map.end()) {
                                fprintf(stderr, "Group \"%s\" defines triggers to non-existant field \"%s\"\n",
                                        info.name.c_str(), target.c_str());
                                continue;
                            }
                            const GroupMemberInfo& targetmem = info.members[it3x->second];

                            // and finally, update source BitSet
                            srcmem.triggers.insert(targetmem.index);
                            if(PDBProviderDebug>2)
                                fprintf(stderr, "%s, ", info.members[targetmem.index].pvfldname.c_str());
                        }
                    }

                    if(PDBProviderDebug>2) fprintf(stderr, "\n");
                }
            } else {
                if(PDBProviderDebug>1) fprintf(stderr, "  pdb default triggers for '%s'\n", info.name.c_str());

                FOREACH(it2, end2, info.members) {
                    GroupMemberInfo& mem = *it2;

                    mem.triggers.insert(mem.index); // default is self trigger
                }
            }
        }
    }

    void setAtomic(const char *value)
    {
        GroupInfo::tribool V = GroupInfo::Unset;
        if(epicsStrCaseCmp(value,"yes")==0) V = GroupInfo::True;
        else if(epicsStrCaseCmp(value,"no")==0) V = GroupInfo::False;
        else {
            fprintf(stderr, "Ignoring unknown value for pdbAtomic \"%s\".\n", value);
        }

        if(V==GroupInfo::Unset) return;

        if(curgroup->atomic!=GroupInfo::Unset && curgroup->atomic!=V)
            fprintf(stderr, "  pdb atomic setting inconsistent '%s'\n",
                    curgroup->name.c_str());

        curgroup->atomic=V;

        if(PDBProviderDebug>2)
            fprintf(stderr, "  pdb atomic '%s' %s\n",
                    curgroup->name.c_str(), curgroup->atomic ? "YES" : "NO");
    }

    PDBProcessor() : curgroup(NULL)
    {
        /* syntax:
         *   info(pdbGroup, "<groupname>|field=FLD|other=FLD2?{}|...")
         *     eg. connect PVA "<groupname>.field" to "<recordname>.FLD"
         *   info(pdbTrigger, "<groupname>|field>field,other,...|field2>*")
         *     eg. subscribe to "<recordname>.FLD" (aka ".field"), on event
         *     post() a PVA monitor with ".FLD"  and ".FLD2" (aka ".other").
         *     If no triggers specified, then every field subscribed to trigger itself
         *   info(pdbAtomic, "<groupname>|...")
         *     Allowed values are "YES" or "NO".  Defaults to "NO"
         *     Whether triggers use multi or single record locking.
         *     Defines default to get/put operations, but individual
         *     requests may override.
         */
        for(pdbRecordIterator rec; !rec.done(); rec.next())
        {
            bool firsttag = true;
            for(pdbInfoIterator info(rec); !info.done(); info.next()) {
                if(firsttag) {
                    recbase = rec.record()->name;
                    recbase += ".";
                    firsttag = false;
                }

                const char *value = info.value();
                const char *tagname = info.name();

                try {
                    if(strncmp(tagname, "pdbGroup", 8)==0) {
                        setupGroup(&value);
                        addMappings(value);

                    } else if(strncmp(tagname, "pdbTrigger", 10)==0) {
                        setupGroup(&value);
                        addTriggers(value);

                    } else if(strncmp(tagname, "pdbAtomic", 9)==0) {
                        setupGroup(&value);
                        setAtomic(value);
                    }
                }catch(std::exception& e){
                    fprintf(stderr, "Error processing PDB info(%s, \"%s\") for record \"%s\": %s\n",
                            tagname, value, rec.record()->name, e.what());
                }
            }
        }
        resolveTriggers();
    }

};
}

PDBProvider::PDBProvider()
{
    PDBProcessor proc;
    pvd::FieldCreatePtr fcreate(pvd::getFieldCreate());
    pvd::PVDataCreatePtr pvbuilder(pvd::getPVDataCreate());

    pvd::StructureConstPtr _options(fcreate->createFieldBuilder()
                                    ->addNestedStructure("_options")
                                        ->add("queueSize", pvd::pvUInt)
                                        ->add("atomic", pvd::pvBoolean)
                                    ->endNested()
                                    ->createStructure());

    // assemble group PVD structure definitions and build dbLockers
    FOREACH(it, end, proc.groups)
    {
        GroupInfo &info=it->second;
        try{
            if(persist_pv_map.find(info.name)!=persist_pv_map.end())
                throw std::runtime_error("name already in used");

            const size_t nchans = info.members.size();

            PDBGroupPV::shared_pointer pv(new PDBGroupPV());
            pv->weakself = pv;
            pv->name = info.name;

            pv->pgatomic = info.atomic!=GroupInfo::False; // default true if Unset
            pv->monatomic = info.hastriggers;

            pvd::shared_vector<PDBGroupPV::Info> members(nchans);

            std::vector<dbCommon*> records(nchans);

            pvd::FieldBuilderPtr builder(fcreate->createFieldBuilder());
            builder->add("record", _options);

            for(size_t i=0; i<nchans; i++)
            {
                GroupMemberInfo &mem = info.members[i];
                PDBGroupPV::Info& info = members[i];

                DBCH chan(mem.pvname);

                builder->add(mem.pvfldname, PVIF::dtype(chan));

                info.attachment = mem.pvfldname;
                info.chan.swap(chan);

                info.triggers.reserve(mem.triggers.size());
                FOREACH(idx, endidx, mem.triggers) {
                    info.triggers.push_back(*idx);
                }

                records[i] = dbChannelRecord(info.chan);
            }
            pv->members.swap(members);

            pv->fielddesc = builder->createStructure();
            pv->complete = pvbuilder->createPVStructure(pv->fielddesc);

            pv->complete->getSubFieldT<pvd::PVBoolean>("record._options.atomic")->put(pv->monatomic);

            DBManyLock L(&records[0], records.size(), 0);
            pv->locker.swap(L);

            for(size_t i=0; i<nchans; i++)
            {
                PDBGroupPV::Info& info = pv->members[i];

                if(info.triggers.empty()) continue;

                std::vector<dbCommon*> trig_records(info.triggers.size());
                for(size_t idx=0; idx<trig_records.size(); idx++) {
                    trig_records[idx] = records[info.triggers[idx]];
                }

                DBManyLock L(&trig_records[0], trig_records.size(), 0);
                info.locker.swap(L);
            }

            persist_pv_map[info.name] = pv;

        }catch(std::exception& e){
            fprintf(stderr, "%s: pdbGroup not created: %s\n", info.name.c_str(), e.what());
        }
    }

    event_context = db_init_events();
    if(!event_context)
        throw std::runtime_error("Failed to create dbEvent context");
    int ret = db_start_events(event_context, "PDB-event", NULL, NULL, epicsThreadPriorityCAServerLow-1);
    if(ret!=DB_EVENT_OK)
        throw std::runtime_error("Failed to stsart dbEvent context");

    // setup group monitors
    try {
        FOREACH(it, end, persist_pv_map)
        {
            const PDBPV::shared_pointer& ppv = it->second;
            PDBGroupPV *pv = dynamic_cast<PDBGroupPV*>(ppv.get());
            if(!pv)
                continue;

            // prepare for monitor

            size_t i=0;
            FOREACH(it, end, pv->members)
            {
                PDBGroupPV::Info& info = *it;
                info.evt_VALUE.index = info.evt_PROPERTY.index = i++;
                info.evt_VALUE.self = info.evt_PROPERTY.self = pv;

                info.pvif.reset(PVIF::attach(info.chan,
                                             pv->complete->getSubFieldT<pvd::PVStructure>(info.attachment)));

                info.evt_PROPERTY.create(event_context, info.chan, &pdb_group_event, DBE_PROPERTY);

                if(!info.triggers.empty()) {
                    printf("##### setup VALUE %s\n", dbChannelName(info.chan));
                    info.evt_VALUE.create(event_context, info.chan, &pdb_group_event, DBE_VALUE|DBE_ALARM);
                } else
                    printf("##### skip  VALUE %s\n", dbChannelName(info.chan));
            }
        }
    }catch(...){
        db_close_events(event_context);
        // TODO, remove PV and continue?
        throw;
    }
}

PDBProvider::~PDBProvider()
{
    {
        epicsGuard<epicsMutex> G(transient_pv_map.mutex());
        if(event_context) {
            /* Explicit destroy to ensure that the dbEventCtx
             * is free'd from the event thread.
             */
            errlogPrintf("Warning: PDBProvider free'd without destroy().  Possible race condition\nn");
        }
    }
    destroy();
}

void PDBProvider::destroy()
{
    dbEventCtx ctxt = NULL;
    persist_pv_map_t ppv;
    {
        epicsGuard<epicsMutex> G(transient_pv_map.mutex());
        persist_pv_map.swap(ppv);
        std::swap(ctxt, event_context);
    }
    ppv.clear(); // indirectly calls all db_cancel_events()
    if(ctxt) db_close_events(ctxt);
}

std::string PDBProvider::getProviderName() { return "PDBProvider"; }

namespace {
struct ChannelFindRequesterNOOP : public pva::ChannelFind
{
    const pva::ChannelProvider::weak_pointer provider;
    ChannelFindRequesterNOOP(const pva::ChannelProvider::shared_pointer& prov) : provider(prov) {}
    virtual ~ChannelFindRequesterNOOP() {}
    virtual void destroy() {}
    virtual std::tr1::shared_ptr<pva::ChannelProvider> getChannelProvider() { return provider.lock(); }
    virtual void cancel() {}
};
}

pva::ChannelFind::shared_pointer
PDBProvider::channelFind(const std::string &channelName, const pva::ChannelFindRequester::shared_pointer &requester)
{
    pva::ChannelFind::shared_pointer ret(new ChannelFindRequesterNOOP(shared_from_this()));

    bool found = false;
    {
        epicsGuard<epicsMutex> G(transient_pv_map.mutex());
        if(persist_pv_map.find(channelName)!=persist_pv_map.end()
                || transient_pv_map.find(channelName)
                || dbChannelTest(channelName.c_str())==0)
            found = true;
    }
    requester->channelFindResult(pvd::Status(), ret, found);
    return ret;
}

pva::ChannelFind::shared_pointer
PDBProvider::channelList(pva::ChannelListRequester::shared_pointer const & requester)
{
    pva::ChannelFind::shared_pointer ret;
    requester->channelListResult(pvd::Status(pvd::Status::STATUSTYPE_ERROR, "not supported"),
                                 ret, pvd::PVStringArray::const_svector(), true);
    return ret;
}

pva::Channel::shared_pointer
PDBProvider::createChannel(std::string const & channelName,
                           pva::ChannelRequester::shared_pointer const & channelRequester,
                           short priority)
{
    return createChannel(channelName, channelRequester, priority, "???");
}

pva::Channel::shared_pointer
PDBProvider::createChannel(std::string const & channelName,
                                                               pva::ChannelRequester::shared_pointer const & requester,
                                                               short priority, std::string const & address)
{
    pva::Channel::shared_pointer ret;
    PDBPV::shared_pointer pv;
    pvd::Status status;
    {
        epicsGuard<epicsMutex> G(transient_pv_map.mutex());

        pv = transient_pv_map.find(channelName);
        if(!pv) {
            persist_pv_map_t::const_iterator it=persist_pv_map.find(channelName);
            if(it!=persist_pv_map.end()) {
                pv = it->second;
            }
        }
        if(!pv) {
            dbChannel *pchan = dbChannelCreate(channelName.c_str());
            if(pchan) {
                DBCH chan(pchan);
                pv.reset(new PDBSinglePV(chan, shared_from_this()));
                transient_pv_map.insert(channelName, pv);
                PDBSinglePV::shared_pointer spv = std::tr1::static_pointer_cast<PDBSinglePV>(pv);
                spv->weakself = spv;
                spv->activate();
            }
        }
    }
    if(pv) {
        ret = pv->connect(shared_from_this(), requester);
    }
    if(!ret) {
        status = pvd::Status(pvd::Status::STATUSTYPE_ERROR, "not found");
    }
    requester->channelCreated(status, ret);
    return ret;
}
