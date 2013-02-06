#ifndef HAZELCAST_COMMAND
#define HAZELCAST_COMMAND

#include "../serialization/DataInput.h"
#include "../serialization/DataOutput.h"
#include <vector>

namespace hazelcast{
namespace client{
namespace protocol{
//
//enum CommandType{
//    AUTH, OK, ERROR, INSTANCES, MEMBERS, CLUSTERTIME, PARTITIONS, TRXCOMMIT, TRXROLLBACK, TRXBEGIN,
//    DESTROY,UNKNOWN, EVENT,
//
//    MGET, MGETALL, MPUT, MTRYPUT, MSET, MPUTTRANSIENT, MPUTANDUNLOCK, MREMOVE, MREMOVEITEM,
//    MCONTAINSKEY, MCONTAINSVALUE, ADDLISTENER, MEVENT, REMOVELISTENER, KEYSET, MENTRYSET, MGETENTRY,
//    MLOCK, MISLOCKED, MUNLOCK,MTRYLOCK, MLOCKMAP, MUNLOCKMAP, MFORCEUNLOCK, MPUTALL, MPUTIFABSENT,
//    MREMOVEIFSAME, MREPLACEIFNOTNULL, MREPLACEIFSAME,MTRYLOCKANDGET, MTRYREMOVE, MFLUSH, MEVICT,
//    MADDLISTENER, MREMOVELISTENER, MSIZE, MADDINDEX, MISKEYLOCKED,
//
//    QOFFER, QPUT, QPOLL, QTAKE, QSIZE, QPEEK, QREMOVE, QREMCAPACITY, QENTRIES, QADDLISTENER,
//    QREMOVELISTENER, QEVENT,
//
//    CDLAWAIT, CDLGETCOUNT, CDLGETOWNER, CDLSETCOUNT, CDLCOUNTDOWN,
//
//    SEMATTACHDETACHPERMITS, SEMCANCELACQUIRE, SEMDESTROY, SEM_DRAIN_PERMITS, SEMGETATTACHEDPERMITS,
//    SEMGETAVAILPERMITS, SEMREDUCEPERMITS, SEMRELEASE, SEMTRYACQUIRE,
//
//    LOCK, TRYLOCK, UNLOCK, FORCEUNLOCK, ISLOCKED,
//
//    SADD, LADD,
//    MMPUT, MMREMOVE, MMVALUECOUNT, MMSIZE, MMCONTAINSENTRY, MMCONTAINSKEY, MMCONTAINSVALUE, MMKEYS,
//    MMGET, MMLOCK, MMUNLOCK, MMTRYLOCK, MMADDLISTENER, MMREMOVELISTENER,
//    ADDANDGET, GETANDSET, COMPAREANDSET, GETANDADD,
//    NEWID,
//    TPUBLISH, TADDLISTENER, TREMOVELISTENER,  MESSAGE
//};    
class Command{
public:
    virtual void writeCommand(hazelcast::client::serialization::DataOutput& dataOutput) = 0;
    virtual void readHeaderLine(std::string dataInput) = 0;
    virtual void readSizeLine(std::string sizeInBytes) = 0;
    virtual void readResult(hazelcast::client::serialization::DataInput& dataInput) = 0;
    virtual int nResults() = 0;
    virtual int resultSize(int i) = 0;
/*
    void setFlag(int flag) {
        this->flag = flag;
        isFlagSet = true;
    };

    int getFlag() const {
        return flag;
    };

    void setThreadId(int threadId) {
        this->threadId = threadId;
        isThreadIdSet = true;
    };

    int getThreadId() const {
        return threadId;
    }

    bool isFlagSet() const {
        return isFlagSet;
    }

    bool isThreadIdSet() const {
        return isThreadIdSet;
    };
private:
     bool isThreadIdSet = false;
     bool isFlagSet = false;
     int threadId;
     int flag;
*/
};


}}}


#endif /* HAZELCAST_COMMAND */