// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "../Ethereum/Address.h"
#include "../proto/VeChain.pb.h"
#include "../uint256.h"

namespace TW::VeChain {

class Clause {
  public:
    Ethereum::Address to;
    uint256_t value;
    Data data;

    Clause(Ethereum::Address to, uint256_t value, const Data& data = {})
        : to(std::move(to)), value(std::move(value)), data(std::move(data)) {}

    /// Decodes from a proto representation.
    Clause(const Proto::Clause& proto)
        : Clause(Ethereum::Address(proto.to()), load(proto.value()),
                 Data(proto.data().begin(), proto.data().end())) {}
};

} // namespace TW::VeChain
