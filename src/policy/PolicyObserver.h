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

#ifndef AIR_POLICY_OBSERVER_H
#define AIR_POLICY_OBSERVER_H

#include "src/lib/Design.h"
#include "src/lib/Protocol.h"
#include "src/policy/RuleManager.h"

namespace policy
{
class Observer : public lib_design::Observer
{
public:
    explicit Observer(RuleManager* new_rule_manager)
    : rule_manager(new_rule_manager)
    {
    }
    virtual ~Observer(void)
    {
    }
    virtual void Update(uint32_t type1, uint32_t type2, uint32_t value1,
        uint32_t value2, int pid, int cmd_type, int cmd_order);
    virtual void Handle(void);

private:
    RuleManager* rule_manager{nullptr};
};

} // namespace policy

#endif // AIR_POLICY_OBSERVER_H
