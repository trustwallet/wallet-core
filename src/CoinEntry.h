// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>

#include "Data.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include <string>
#include <vector>

namespace TW {

/// Interface for coin-specific entry, used to dispatch calls to coins
/// Implement this for all coins.
class CoinEntry {
public:
    // Report the coin types this implementation is responsible of
    virtual const std::vector<TWCoinType> coinTypes() const = 0;
    virtual bool validateAddress(TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const = 0;
    // normalizeAddress is optional, it may leave this default, no-change implementation
    virtual std::string normalizeAddress(TWCoinType coin, const std::string& address) const { return address; }
    virtual std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const = 0;
    // Signing
    virtual void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const = 0;
    virtual bool supportsJSONSigning() const { return false; }
    // It is optional, Signing JSON input with private key
    virtual std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const { return ""; }
    // Planning, for UTXO chains, in preparation for signing
    // It is optional, only UTXO chains need it, default impl. leaves empty result.
    virtual void plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const { return; }
};

// In each coin's Entry.cpp the specific types of the coin are used, this template enforces the Signer implement:
// static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept;
// Note: use output parameter to avoid unneeded copies
template <typename Signer, typename Input>
void signTemplate(const Data& dataIn, Data& dataOut) {
    auto input = Input();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    auto serializedOut = Signer::sign(input).SerializeAsString();
    dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
}

// Note: use output parameter to avoid unneeded copies
template <typename Planner, typename Input>
void planTemplate(const Data& dataIn, Data& dataOut) {
    auto input = Input();
    input.ParseFromArray(dataIn.data(), (int)dataIn.size());
    auto serializedOut = Planner::plan(input).SerializeAsString();
    dataOut.insert(dataOut.end(), serializedOut.begin(), serializedOut.end());
}

} // namespace TW
