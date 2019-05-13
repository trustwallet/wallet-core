// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <vector>
#include <cstdint>
#include <iterator>
#include "../Data.h"
#include "../ReadData.h"

namespace TW::NEO {
    class ISerializable {
    public:
        virtual ~ISerializable() {}
        virtual int64_t size() const = 0;
        virtual Data serialize() const = 0;
        virtual void deserialize(const Data &data, int initial_pos = 0) = 0;

        template<class T>
        static inline Data serialize(const std::vector<T> & data) {
            return serialize(data.begin(), data.end());
        }

        template<class T>
        static inline Data serialize(const T* data, int size) {
            return serialize(data, data + size);
        }

        template<class T>
        static inline Data serialize(std::iterator<std::input_iterator_tag, T> begin, std::iterator<std::input_iterator_tag, T> end) {
            Data resp(writeVarUInt(end - begin));
            for (auto it = begin; it < end; ++it) {
                append(resp, it->serialize());
            }
            return resp;
        }

        /*template<class T>
        static inline Data deserialize(const Data &data, int initial_pos = 0) {
            ulong size = readVarUInt(data, initial_pos);
            return serialize(data.begin(), data.end());
        }*/
        
    };

} // namespace TW::NEO
