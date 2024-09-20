// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "TrustWalletCore/TWCoinType.h"

namespace TW::Bitcoin {

class Psbt {
public:
    /// Signs a PSBT (Partially Signed Bitcoin Transaction) specified by the signing input and coin type.
    static Data sign(const Data& input, TWCoinType coin);

    /// Plans a PSBT (Partially Signed Bitcoin Transaction).
    /// Can be used to get the transaction detailed decoded from PSBT.
    static Data plan(const Data& input, TWCoinType coin);
};

} // namespace TW::Bitcoin
