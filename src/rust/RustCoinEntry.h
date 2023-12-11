// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "CoinEntry.h"

namespace TW::Rust {

class RustCoinEntry : public CoinEntry {
public:
    ~RustCoinEntry() noexcept override = default;
    bool validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const override;
    std::string normalizeAddress(TWCoinType coin, const std::string& address) const override;
    std::string deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const override;
    Data addressToData(TWCoinType coin, const std::string& address) const override;
    void sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const override;

    Data preImageHashes(TWCoinType coin, const Data& txInputData) const override;
    void compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const override;
};

} // namespace TW::Rust
