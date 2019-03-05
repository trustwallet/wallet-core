// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Ethereum/Address.h"
#include "../proto/VeChain.pb.h"
#include "../Data.h"
#include "../uint256.h"

namespace TW {
namespace VeChain {

class Clause {
public:
    Ethereum::Address to;
    uint256_t value;
    Data data;

    Clause() = default;
    Clause(Ethereum::Address to, uint256_t value, Data data = {}) : to(to), value(value), data(data) {}

    /// Decodes from a proto representation.
    Clause(const Proto::Clause& proto)
        : Clause(
            Ethereum::Address(proto.to()),
            load(proto.value()),
            Data(proto.data().begin(), proto.data().end())
        )
    {}
};

}} // namespace
