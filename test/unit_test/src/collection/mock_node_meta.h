
#include "src/meta/NodeMeta.h"

class MockNodeMetaGetter : public meta::NodeMetaGetter
{
public:
    virtual ~MockNodeMetaGetter()
    {
    }
    virtual inline bool
    Run(uint32_t node_index) const
    {
        return true;
    }
    virtual inline air::ProcessorType
    ProcessorType(uint32_t node_index) const
    {
        air::ProcessorType ptype = air::ProcessorType::PROCESSORTYPE_NULL;

        switch (node_index)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                ptype = air::ProcessorType::PERFORMANCE;
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                ptype = air::ProcessorType::LATENCY;
                break;
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
                ptype = air::ProcessorType::QUEUE;
                break;
            case 15:
                ptype = air::ProcessorType::UTILIZATION;
                break;
            case 16:
                ptype = air::ProcessorType::COUNT;
                break;
            case 17:
                ptype = air::ProcessorType::HISTOGRAM;
                break;
            default:
                break;
        }

        return ptype;
    }
    virtual inline uint32_t
    GroupId(uint32_t node_index) const
    {
        return group_id[node_index];
    }
    virtual inline uint32_t
    IndexSize(uint32_t nid) const
    {
        return 10;
    }
    virtual inline uint32_t
    FilterSize(uint32_t nid) const
    {
        return 10;
    }

private:
    uint32_t group_id[7]{1, 0, 1, 3, 2, 0, 1};
};
