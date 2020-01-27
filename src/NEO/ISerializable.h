// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../BinaryCoding.h"
#include "ReadData.h"

namespace TW::NEO {

class ISerializable {
  public:
    virtual ~ISerializable() {}
    virtual int64_t size() const = 0;
    virtual Data serialize() const = 0;
    virtual void deserialize(const Data &data, int initial_pos = 0) = 0;

    template<class T>
    static inline Data serialize(const std::vector <T> &data) {
        Data resp;
        encodeVarInt(data.size(), resp);
        for (auto it = data.begin(); it < data.end(); ++it) {
            append(resp, it->serialize());
        }
        return resp;
    }

    template<class T>
    static inline Data serialize(const T *data, int size) {
        Data resp;
        encodeVarInt(uint64_t(size), resp);
        for (int i = 0; i < size; ++i) {
            append(resp, data[i].serialize());
        }
        return resp;
    }

    template<class T>
    static inline Data
    serialize(std::iterator <std::input_iterator_tag, T> begin, std::iterator <std::input_iterator_tag, T> end) {
        Data resp;
        encodeVarInt(uint64_t(end - begin), resp);
        for (auto it = begin; it < end; ++it) {
            append(resp, it->serialize());
        }
        return resp;
    }

    template<class T>
    static inline int deserialize(std::vector <T> &resp, const Data &data, int initial_pos = 0) {
        uint64_t size = readVar<uint64_t>(data, initial_pos, INT_MAX);
        if (size < 0) {
            throw std::invalid_argument("ISerializable::deserialize ArgumentOutOfRangeException");
        }

        initial_pos += varIntSize(size);
        for (uint64_t i = 0; i < size; ++i) {
            T value;
            value.deserialize(data, initial_pos);
            resp.push_back(value);
            initial_pos += value.size();
        }

        return initial_pos;
    }

};

} // namespace TW::NEO
