// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>

#include "Data.h"
#include "PublicKey.h"

#include <string> // to avoid std::string, to simplify the interface implemented by each coin

namespace TW {

/// Interface for coin-specific entry, used to dispatch calls to coins
/// Implement this for all coins.
class CoinEntry {
public:
    virtual TWCoinType coinType() const = 0;
    virtual bool validateAddress(const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const = 0;
    // normalizeAddress is optional, it may leave this default, no-change implementation
    virtual std::string normalizeAddress(const std::string& address) const { return address; }
    virtual std::string deriveAddress(const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const = 0;
    // Signing
    virtual void sign(const Data& dataIn, Data& dataOut) const = 0;
    // Planning, for UTXO chains, in preparation for signing
    // It is optional, only UTXO chains need it, default impl. leaves empty result.
    virtual void plan(const Data& dataIn, Data& dataOut) const { return; }
};

// TODO: Is this template really needed?  Each typed Entry.cpp can include it's own typed version anyways
// Note: use output parameter to avoid unneeded copies
template <typename Signer, typename Input>
void AnySignTempl(const Data& dataIn, Data& dataOut) {
    auto input = Input();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    auto serializedOut = Signer::sign(input).SerializeAsString();
    dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
}

// Note: use output parameter to avoid unneeded copies
template <typename Signer, typename Input>
void planTemplate(const Data& dataIn, Data& dataOut) {
    auto input = Input();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    auto serializedOut = Signer::plan(input).SerializeAsString();
    dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
}

} // namespace TW
