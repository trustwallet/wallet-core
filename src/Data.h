// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint>
#include <vector>

namespace TW {

using byte = std::uint8_t;
using Data = std::vector<byte>;

inline void append(Data& data, const Data& suffix) {
    data.insert(data.end(), suffix.begin(), suffix.end());
}

/// Determines if a byte array has a specific prefix.
template <typename T>
inline bool has_prefix(const Data& data, T& prefix) {
    return std::equal(prefix.begin(), prefix.end(), data.begin(), data.begin() + std::min(data.size(), prefix.size()));
}

inline Data readBytes(const Data &from, int max, int initial_pos = 0);
inline uint16_t readUInt16(const Data &from, int initial_pos);
/// read int16 in little-endian format
inline int16_t readInt16(const Data &from, int initial_pos);
inline int64_t readInt64(const Data &from, int initial_pos);
inline uint64_t readUInt64(const Data &from, int initial_pos);

inline Data writeUshort(ushort v);
inline Data writeShort(short v);
inline Data writeUint(uint v);
inline Data writeInt(int v);
inline Data writeUlong(ulong v);
inline Data writeLong(long v);

template<class T>
static std::vector<T> concat(const std::vector<T>& v1, const std::vector<T>& v2)
{
    std::vector<T> v(v1);
    v.insert(v.end(), v2.begin(), v2.end());
    return std::move(v);
}

class types
{
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
