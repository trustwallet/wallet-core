// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

namespace TW::Tezos {

/// Entry point for implementation of Tezos coin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry: public CoinEntry {
public:
    virtual bool validateAddress(TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const override;    
    virtual std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const override;
    virtual void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;
    virtual bool supportsJSONSigning() const override { return true; }
    virtual std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const override;

    virtual Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
    virtual void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
};

} // namespace TW::Tezos
