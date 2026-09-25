// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/RustCoinEntry.h"

namespace TW::Bitcoin {

/// A helper Bitcoin Entry migrated to Rust partly used in some cases,
/// for example, in Taproot address generation.
/// TODO remove when `Bitcoin` is migrated to Rust completely.
class EntryV2: public Rust::RustCoinEntry {
};

/// Bitcoin entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific
/// includes in this file
class Entry : public CoinEntry {
public:
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const final;
    std::string normalizeAddress(TWCoinType coin, const std::string& address) const final;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const final;
    Data addressToData(TWCoinType coin, const std::string& address) const final;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const final;
    void plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const final;

    Data preImageHashes(TWCoinType coin, const Data& txInputData) const final;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                 const std::vector<PublicKey>& publicKeys, Data& dataOut) const final;
    // Note: buildTransactionInput is not implemented for Binance chain with UTXOs
};

} // namespace TW::Bitcoin
