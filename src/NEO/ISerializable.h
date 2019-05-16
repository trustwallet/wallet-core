// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <vector>
#include <iterator>
#include <cstdint>
#include <climits>
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
            Data resp(writeVar<uint64_t>(data.size()));
            for (auto it = data.begin(); it < data.end(); ++it) {
                append(resp, it->serialize());
            }
            return resp;
        }

        template<class T>
        static inline Data serialize(const T* data, int size) {
            Data resp(writeVar<uint64_t>(size));
            for (int i = 0; i < size; ++i) {
                append(resp, data[i].serialize());
            }
            return resp;
        }

        template<class T>
        static inline Data serialize(std::iterator<std::input_iterator_tag, T> begin, std::iterator<std::input_iterator_tag, T> end) {
            Data resp(writeVar<uint64_t>(end - begin));
            for (auto it = begin; it < end; ++it) {
                append(resp, it->serialize());
            }
            return resp;
        }

        template<class T>
        static inline int deserialize(std::vector<T> &resp, const Data &data, int initial_pos = 0) {
            uint64_t size = readVar<uint64_t>(data, initial_pos, INT_MAX);
            if (size < 0) {
                throw std::invalid_argument("ISerializable::deserialize ArgumentOutOfRangeException");
            }
            if (size < 0xFD) {
                initial_pos += 1;
            } else if (size <= 0xFFFF) {
                initial_pos += 2;
            } else if (size <= 0xFFFFFFFF) {
                initial_pos += 4;
            } else {
                initial_pos += 8;
            }

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
