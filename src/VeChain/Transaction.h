// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Clause.h"
#include "../Data.h"

#include <cstdint>
#include <vector>

namespace TW {
namespace VeChain {

class Transaction {
public:
    std::uint8_t chainTag;
    std::uint64_t blockRef;
    std::uint32_t expiration;
    std::vector<Clause> clauses;
    std::uint8_t gasPriceCoef;
    std::uint64_t gas;
    Data dependOn;
    std::uint64_t nonce;
    std::vector<Data> reversed;
    Data signature;
    
public:
    /// Encodes the transaction.
    Data encode() const noexcept;
};

}} // namespace
