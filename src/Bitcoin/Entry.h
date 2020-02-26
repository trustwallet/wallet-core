// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../CoinEntry.h"

namespace TW::Bitcoin {

/// Bitcoin entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry: public CoinEntry {
public:
    virtual TWCoinType coinType() const { return TWCoinTypeBitcoin; }
    virtual bool validateAddress(const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const;
    // normalizeAddress: do not override
    virtual std::string deriveAddress(const PublicKey& publicKey, TW::byte p2pkh, const char* hrp) const;
    virtual void sign(const Data& dataIn, Data& dataOut) const;
    virtual void plan(const Data& dataIn, Data& dataOut) const;
};

} // namespace TW::Bitcoin
