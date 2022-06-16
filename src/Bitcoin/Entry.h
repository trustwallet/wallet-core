// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../CoinEntry.h"

namespace TW::Bitcoin {

/// Bitcoin entry dispatcher.
/// Note: do not put the implementation here (no matter how simple), to avoid having coin-specific
/// includes in this file
class Entry : public CoinEntry {
public:
    bool validateAddress(TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh,
                         const char* hrp) const final;
    std::string normalizeAddress(TWCoinType coin, const std::string& address) const final;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte p2pkh,
                              const char* hrp) const final {
        return deriveAddress(coin, TWDerivationDefault, publicKey, p2pkh, hrp);
    }
    std::string deriveAddress(TWCoinType coin, TWDerivation derivation, const PublicKey& publicKey,
                              TW::byte p2pkh, const char* hrp) const final;
    Data addressToData(TWCoinType coin, const std::string& address) const;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const final;
    void plan(TWCoinType coin, const Data& dataIn, Data& dataOut) const final;

    Data preImageHashes(TWCoinType coin, const Data& txInputData) const final;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures,
                 const std::vector<PublicKey>& publicKeys, Data& dataOut) const final;
    // Note: buildTransactionInput is not implemented for Binance chain with UTXOs
};

} // namespace TW::Bitcoin
