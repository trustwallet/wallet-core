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

using namespace std;

/// Interface for coin-specific entry, used to dispatch calls to coins
/// Implement this for all coins.
class CoinEntry {
public:
    virtual TWCoinType coinType() const = 0;
    virtual bool validateAddress(const string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const = 0;
    // normalizeAddress is optional, it may leave this default, no-change implementation
    virtual string normalizeAddress(const string& address) const { return address; }
    virtual string deriveAddress(const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const = 0;
    // TODO signing
    //Data sign(const Data& signingInputData) const = 0;
};

} // namespace TW
