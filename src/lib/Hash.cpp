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

#include "src/lib/Hash.h"

template<typename K>
air::HashMap<K>::HashMap(uint32_t capacity)
{
    this->capacity = capacity;
    hash_arr = new HashNode<K>*[capacity];
    for (uint32_t i = 0; i < capacity; i++)
    {
        hash_arr[i] = new HashNode<K>(-1, true);
    }
}

template<typename K>
air::HashMap<K>::~HashMap(void)
{
    for (uint32_t i = 0; i < capacity; i++)
    {
        if (hash_arr[i] != nullptr)
        {
            delete hash_arr[i];
            hash_arr[i] = nullptr;
        }
    }
    delete[] hash_arr;
}

// explicit instantiation
template class air::HashMap<uint64_t>;
