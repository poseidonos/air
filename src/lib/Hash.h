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

#ifndef AIR_HASH_H
#define AIR_HASH_H

#include <cstdint>
#include <string>

namespace air
{
template<typename K>
class HashNode
{
public:
    HashNode(K new_key, bool new_valid)
    : key(new_key),
      valid(new_valid)
    {
    }
    K key;
    bool valid;
};

template<typename K>
class HashMap
{
public:
    explicit HashMap(uint32_t new_capacity);
    virtual ~HashMap(void);
    virtual inline uint32_t
    InsertHashNode(K key)
    {
        if (capacity == GetHashSize())
        {
            return capacity;
        }
        uint32_t hash_index = _HashCode(key);
        while (hash_arr[hash_index]->key != key && hash_arr[hash_index]->valid == false)
        {
            hash_index++;
            hash_index %= capacity;
        }
        if (hash_arr[hash_index]->valid == true)
        {
            size++;
        }
        hash_arr[hash_index]->key = key;
        hash_arr[hash_index]->valid = false;
        return hash_index;
    }
    virtual inline bool
    DeleteHashNode(K key)
    {
        uint32_t hash_index = _HashCode(key);
        uint32_t count = 0;
        while (count < capacity)
        {
            if (hash_arr[hash_index]->key == key)
            {
                hash_arr[hash_index]->key = -1;
                hash_arr[hash_index]->valid = true;
                size--;
                return true;
            }
            hash_index++;
            hash_index %= capacity;
            count++;
        }
        return false;
    }
    virtual inline uint32_t
    GetHashIndex(K key)
    {
        uint32_t hash_index = _HashCode(key);
        uint32_t count = 0;
        while (count < capacity)
        {
            if (hash_arr[hash_index]->key == key)
            {
                return hash_index;
            }
            hash_index++;
            hash_index %= capacity;
            count++;
        }
        return capacity;
    }
    virtual inline K
    GetHashKey(uint32_t hash_index)
    {
        if (hash_index < capacity)
        {
            return hash_arr[hash_index]->key;
        }
        return npos;
    }
    virtual inline uint32_t
    GetHashSize(void)
    {
        return size;
    }
    static const uint64_t npos{0xFFFFFFFFFFFFFFFF};

private:
    uint32_t
    _HashCode(K key)
    {
        uint64_t x = (uint64_t)key;
        if (x < capacity)
        {
            return x;
        }
        x = ((x >> 16) ^ x) * 0x119de1f3;
        x = ((x >> 16) ^ x) * 0x119de1f3;
        x = (x >> 16) ^ x;
        return x % capacity;
    }

    HashNode<K>** hash_arr{nullptr};
    uint32_t capacity{0};
    uint32_t size{0};
};

} // namespace air

#endif // AIR_HASH_H
