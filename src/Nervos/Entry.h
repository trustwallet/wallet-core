// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

using namespace TW;

namespace TW::Nervos {

/// Entry point for implementation of Nervos coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific
/// includes in this file
class Entry : public CoinEntry {
public:
    bool validateAddress(TWCoinType coin, const std::string& address, byte p2pkh, byte p2sh,
                         const char* hrp) const final;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, byte p2pkh,
                              const char* hrp) const final;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const final;
    void plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const final;
};

} // namespace TW::Nervos
