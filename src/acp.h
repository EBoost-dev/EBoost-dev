

#include "init.h"
#include "serialize.h"
#include "uint256.h"
#include "tinyformat.h"
#include "net.h"
#include "protocol.h"
#include "netmessagemaker.h"

#define strprintf tfm::format

class uint256;
class CBlock;
class CBlockIndex;
class CSyncCheckpoint;

// extern ACPDB *acpdb;

extern std::unique_ptr<CConnman> g_connman;
extern uint256 hashSyncCheckpoint;
extern CSyncCheckpoint checkpointMessage;
extern uint256 hashInvalidCheckpoint;
extern CCriticalSection cs_hashSyncCheckpoint;
extern std::string strCheckpointWarning;
extern int64_t ACP_DEPTH;

bool CheckCheckpointPubKey();
bool ResetSyncCheckpoint();
bool CheckSyncCheckpoint(const uint256& hashBlock, const CBlockIndex* pindexPrev);
bool SetCheckpointPrivKey(std::string strPrivKey);
bool WriteSyncCheckpoint(const uint256& hashCheckpoint);
bool SendSyncCheckpoint(uint256 hashCheckpoint);
uint256 AutoSelectSyncCheckpoint();

// Synchronized checkpoint (introduced first in ppcoin)
class CUnsignedSyncCheckpoint
{
public:
    int nVersion;
    uint256 hashCheckpoint;      // checkpoint block

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(hashCheckpoint);
    }

    void SetNull()
    {
        nVersion = 1;
        hashCheckpoint.SetNull();
    }

    std::string ToString() const
    {
        return strprintf(
                "CSyncCheckpoint(\n"
                "    nVersion       = %d\n"
                "    hashCheckpoint = %s\n"
                ")\n",
            nVersion,
            hashCheckpoint.ToString().c_str());
    }

    void print() const
    {
        printf("%s", ToString().c_str());
    }
};

class CSyncCheckpoint : public CUnsignedSyncCheckpoint
{
public:
    static const std::string strMainPubKey;
    static const std::string strTestPubKey;
    static std::string strMasterPrivKey;
    static int acpdepth;

    std::vector<unsigned char> vchMsg;
    std::vector<unsigned char> vchSig;

    CSyncCheckpoint()
    {
        SetNull();
    }

    ADD_SERIALIZE_METHODS;
    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(vchMsg);
        READWRITE(vchSig);
    }

    void SetNull()
    {
        CUnsignedSyncCheckpoint::SetNull();
        vchMsg.clear();
        vchSig.clear();
    }

    bool IsNull() const
    {
        return hashCheckpoint.IsNull();
    }

    uint256 GetHash() const
    {
        return Hash(this->vchMsg.begin(), this->vchMsg.end());
    }

    bool RelayTo(CNode* pnode) const
    {
        const CNetMsgMaker msgMaker(PROTOCOL_VERSION);

        // returns true if wasn't already sent
        if (pnode->hashCheckpointKnown != hashCheckpoint)
        {
            pnode->hashCheckpointKnown = hashCheckpoint;
            g_connman->PushMessage(pnode, msgMaker.Make(NetMsgType::ACP, *this));
            return true;
        }
        return false;
    }

    bool CheckSignature();
    bool ProcessSyncCheckpoint(CNode* pfrom);
};
