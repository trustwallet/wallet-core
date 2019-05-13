// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <ctype.h>
#include "Data.h"

namespace TW {

Data readBytes(const Data &from, int max, int initial_pos = 0);
Data readVarBytes(const Data &from, int initial_pos = 0);
uint16_t readUInt16(const Data &from, int initial_pos = 0);
uint32_t readUInt32(const Data &from, int initial_pos = 0);
uint64_t readUInt64(const Data &from, int initial_pos = 0);
int16_t readInt16(const Data &from, int initial_pos = 0);
int32_t readInt32(const Data &from, int initial_pos = 0);
int64_t readInt64(const Data &from, int initial_pos = 0);
uint64_t readVarUInt(const TW::Data &from, uint64_t max, int initial_pos = 0);
int64_t readVarInt(const TW::Data &from, uint64_t max, int initial_pos = 0);

Data writeVarBytes(const Data &from, int initial_pos = 0);
Data writeUshort(ushort v);
Data writeUint(uint v);
Data writeUlong(ulong v);
Data writeShort(short v);
Data writeInt(int v);
Data writeLong(long v);
Data writeVarUInt(ulong value);
Data writeVarInt(long value);

template<class T>
static std::vector<T> concat(const std::vector<T>& v1, const std::vector<T>& v2) {
    std::vector<T> v(v1);
    v.insert(v.end(), v2.begin(), v2.end());
    return std::move(v);
}

class types {
public:
    template <class T>
    static T MaxValue()
    {
        return std::numeric_limits<T>::max();
    }

    template <class T>
    static T MinValue()
    {
        return std::numeric_limits<T>::min();
    }
};

} // namespace TW
