// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Action.h"
#include "Data.h"

#include <vector>

namespace TW::FIO {

class ChainParams; // forward

/// A Transaction.  Some common fields, and one or more actions.
class Transaction {
public:
    int32_t expiration = 0;
    uint16_t refBlockNumber = 0;
    uint32_t refBlockPrefix = 0;
    std::vector<Action> actions;

    // Set values with truncations
    void set(uint32_t expiryTime, const ChainParams& chainParams);
    // Serailize to binary stream
    void serialize(Data& os) const;
};

} // namespace TW::FIO
