// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

namespace TW::NEO {

/// NEO entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry: public CoinEntry {
public:
    const std::vector<TWCoinType> coinTypes() const override { return {TWCoinTypeNEO}; }
    bool validateAddress(TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const override;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const override;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;
    void plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;
    
    Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
};

} // namespace TW::NEO
