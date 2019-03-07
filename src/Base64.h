// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

namespace TW {
namespace Base64 {

inline Data decode(const std::string& val) {
    using namespace boost::archive::iterators;
    using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
    return boost::algorithm::trim_right_copy_if(Data(It(std::begin(val)), It(std::end(val))), [](char c) {
        return c == '\0';
    });
}

inline std::string encode(const Data& val) {
    using namespace boost::archive::iterators;
    using It = base64_from_binary<transform_width<Data::const_iterator, 6, 8>>;
    auto encoded = std::string(It(std::begin(val)), It(std::end(val)));
    return encoded.append((3 - val.size() % 3) % 3, '=');
}

}} // namespace
