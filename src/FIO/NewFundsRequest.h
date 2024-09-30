// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

#include <string>

namespace TW::FIO {

/// Data class, data encoded in a new funds request
class NewFundsContent {
public:
    /// Public addressed of the payee, on the mentioned blockchain.
    std::string payeePublicAddress;
    /// Amount requested (string)
    std::string amount;
    /// Chain ID for the coin (on case of multiple chains)
    std::string chainCode;
    /// Coin symbol of the amount requested (a.k.a. tokenCode)
    std::string coinSymbol;
    /// Memo free text (optional, may be empty)
    std::string memo;
    /// Optional hash
    std::string hash;
    /// Optional url
    std::string offlineUrl;

    /// Serialize into binary stream (abi)
    void serialize(Data& out) const;

    /// Deserialize, create from binary stream.  Return default (empty fields) on error.
    static NewFundsContent deserialize(const Data& in, size_t& indexInOut);
};

} // namespace TW::FIO
