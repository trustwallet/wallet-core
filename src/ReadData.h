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

    template<class T> T readVar(const TW::Data &from, int initial_pos = 0, const T &max = INT_MAX);
    template<> int64_t readVar(const TW::Data &from, int initial_pos, const int64_t &max);
    template<> uint64_t readVar(const TW::Data &from, int initial_pos, const uint64_t &max);

    template<class T> T readNumber(const TW::Data &from, int initial_pos = 0);
    template<> int16_t readNumber(const TW::Data &from, int initial_pos);
    template<> int32_t readNumber(const TW::Data &from, int initial_pos);
    template<> int64_t readNumber(const TW::Data &from, int initial_pos);
    template<> uint16_t readNumber(const TW::Data &from, int initial_pos);
    template<> uint32_t readNumber(const TW::Data &from, int initial_pos);
    template<> uint64_t readNumber(const TW::Data &from, int initial_pos);

    Data writeVarBytes(const Data &from, int initial_pos = 0);

    template<class T> Data writeVar(const T & value);
    template<> Data writeVar(const int64_t & value);
    template<> Data writeVar(const uint64_t & value);

    template<class T> Data write(const T &v);
    template<> Data write(const int16_t &v);
    template<> Data write(const int32_t &v);
    template<> Data write(const int64_t &v);
    template<> Data write(const uint16_t &v);
    template<> Data write(const uint32_t &v);
    template<> Data write(const uint64_t &v);


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
