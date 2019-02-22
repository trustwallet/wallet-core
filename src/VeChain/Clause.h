// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Ethereum/Address.h"
#include "../Data.h"

#include <boost/multiprecision/cpp_int.hpp>

namespace TW {
namespace VeChain {

class Clause {
public:
    Ethereum::Address to;
    boost::multiprecision::uint256_t value;
    Data data;

    Clause() = default;
    Clause(Ethereum::Address to, boost::multiprecision::uint256_t value, Data data = {}) : to(to), value(value), data(data) {}
};

}} // namespace
