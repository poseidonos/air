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

#ifndef AIR_DETECT_COR_HANDLER_H
#define AIR_DETECT_COR_HANDLER_H

#include "src/lib/Design.h"
#include "src/target_detector/Detector.h"

namespace detect
{
class DetectCoRHandler : public lib_design::AbstractCoRHandler
{
public:
    explicit DetectCoRHandler(Detector* detector): detector(detector)
    {
    }
    virtual ~DetectCoRHandler(void)
    {
    }
    virtual void
    HandleRequest(int option = 0)
    {
        detector->Process();
    }

private:
    Detector* detector {nullptr};
};

} // namespace detect

#endif // AIR_DETECT_COR_HANDLER_H
