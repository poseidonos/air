/*
 *   MIT License
 *
 *   Copyright (c) 2021 Samsung Electronics Corporation
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in all
 *   copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *   SOFTWARE.
 */

#ifndef AIR_RULE_MANAGER_H
#define AIR_RULE_MANAGER_H

#include <queue>
#include <string>

#include "src/lib/Design.h"
#include "src/lib/Msg.h"
#include "src/lib/Protocol.h"
#include "src/lib/Type.h"
#include "src/policy/Ruler.h"

namespace policy
{
class Subject : public lib_design::Subject
{
public:
    virtual ~Subject(void)
    {
    }
    virtual int Notify(uint32_t index, uint32_t type1, uint32_t type2,
        uint32_t value1, uint32_t value2, int pid, int cmd_type, int cmd_order);
};

class RuleManager
{
public:
    RuleManager(Ruler* ruler, policy::Subject* subject)
    : ruler(ruler),
      subject(subject)
    {
    }
    void SetNodeMetaConfig(void* node);
    void SetGlobalConfig(void);
    int UpdateRule(uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2,
        int pid, int cmd_type, int cmd_order);
    void HandleMsg(void);
    void
    EnqueueMsg(uint32_t type1, uint32_t type2, uint32_t value1, uint32_t value2,
        int pid, int cmd_type, int cmd_order)
    {
        msg.push({type1, type2, value1, value2, pid, cmd_type, cmd_order});
    }

private:
    Ruler* ruler {nullptr};
    Subject* subject {nullptr};
    std::queue<lib::MsgEntry> msg;
    //    bool node_link {false};
    //    bool event_link {false};

    //    int pid_        {-1};
    //    int cmd_type_   {-1};
};

} // namespace policy

#endif // AIR_RULE_MANAGER_H
