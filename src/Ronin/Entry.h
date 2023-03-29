// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

namespace TW::Ronin {

/// Entry point for Ronin (EVM side chain)
class Entry final : public CoinEntry {
public:
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const;
     std::string normalizeAddress(TWCoinType coin, const std::string& address) const;
     std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const;
     Data addressToData(TWCoinType coin, const std::string& address) const;
     void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const;
     bool supportsJSONSigning() const { return true; }
     std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const;
};

} // namespace TW::Ronin
