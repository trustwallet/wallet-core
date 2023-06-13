// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

namespace TW::Ethereum {

/// Entry point for Ethereum and Ethereum-fork coins.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific includes in this file
class Entry : public CoinEntry {
public:
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const final;
     std::string normalizeAddress(TWCoinType coin, const std::string& address) const final;
     std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const final;
     Data addressToData(TWCoinType coin, const std::string& address) const final;
     void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;
     bool supportsJSONSigning() const final { return true; }
     std::string signJSON(TWCoinType coin, const std::string& json, const Data& key) const final;

     Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
     void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
     Data buildTransactionInput(TWCoinType coinType, const std::string& from, const std::string& to, const uint256_t& amount, const std::string& asset, const std::string& memo, const std::string& chainId) const final;
};

} // namespace TW::Ethereum
