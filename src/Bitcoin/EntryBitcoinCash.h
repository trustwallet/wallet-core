// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"
#include "Entry.h" // Bitcoin

namespace TW::Bitcoin {

/// BitcoinCash entry dispatcher, based on Bitcoin.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class EntryBitcoinCash: public Entry {
public:
    virtual TWCoinType coinType() const { return TWCoinTypeBitcoinCash; }
    // validateAddress is different
    virtual bool validateAddress(const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const;
    virtual std::string normalizeAddress(const std::string& address) const;
    // deriveAddress is different
    virtual std::string deriveAddress(const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const;
};

} // namespace TW::Bitcoin
