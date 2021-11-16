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

#ifndef AIR_JSONER_H
#define AIR_JSONER_H

#include <map>
#include <stdexcept>
#include <string>
#include <utility>

#include "src/lib/json/Jsondoc.h"

namespace air
{
class JSONer
{
public:
    static JSONdoc&
    JSON(std::string key)
    {
        return _GetInstance()._GetDocument(key);
    }
    static void
    Clear(void)
    {
        _GetInstance()._ClearDoc();
    }
    static JSONdoc&&
    Copy(std::string key)
    {
        JSONdoc* doc = new JSONdoc {JSONtype::OBJECT, true, false};
        _GetInstance()._GetDocument(key).Copy(doc);
        return std::move(*doc);
    }

private:
    JSONer(void)
    {
        docs.clear();
    }
    ~JSONer(void)
    {
        try
        {
            _ClearDoc();
        }
        catch (std::exception& e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    JSONer(const JSONer&) = delete;
    JSONer& operator=(const JSONer&) = delete;

    static JSONer&
    _GetInstance(void)
    {
        static JSONer instance;
        return instance;
    }

    JSONdoc&
    _CreateDocument(std::string key)
    {
        JSONdoc* doc = new JSONdoc {JSONtype::OBJECT, true, false};
        docs.insert({key, doc});
        return *doc;
    }

    JSONdoc&
    _GetDocument(std::string key)
    {
        auto search = docs.find(key);
        if (docs.end() != search)
        {
            return *(search->second);
        }
        else
        {
            return _CreateDocument(key);
        }
    }

    void
    _ClearDoc(void)
    {
        for (const auto& doc : docs)
        {
            doc.second->Clear();
            delete doc.second;
        }
        docs.clear();
    }

    std::map<std::string, JSONdoc*> docs;
};

constexpr auto json = JSONer::JSON;
constexpr auto json_clear = JSONer::Clear;
constexpr auto json_copy = JSONer::Copy;

} // namespace air

#endif // AIR_JSONER_H
