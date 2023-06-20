// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bech32Address.h"

#include <TrustWalletCore/TWCoinType.h>
#include <string>

namespace TW::Binance {

/// Binance address is a Bech32Address, with "bnb" prefix and sha256ripemd hash
class Address: public Bech32Address {
public:
    static const std::string hrpValidator;

    /// Checks if the given `addr` is a valid Binance address and has a known hrp.
    static bool isValid(TWCoinType coin, const std::string& addr);
    /// Checks if the given `addr` is a valid Binance address and has the given `chainHrp`.
    static bool isValid(const std::string& addr, const std::string& chainHrp);

    explicit Address(TWCoinType coin = TWCoinTypeBinance);

    explicit Address(const std::string& chainHrp) : Bech32Address(chainHrp) {}

    /// Initializes an address with a key hash and hrp.
    explicit Address(const Data& keyHash, const std::string& chainHrp) : Bech32Address(chainHrp, keyHash) {}

    /// Initializes an address with a key hash and coin type.
    explicit Address(const Data& keyHash, TWCoinType coin = TWCoinTypeBinance);

    /// Initializes an address with a public key and hrp.
    explicit Address(const PublicKey& publicKey, const std::string& chainHrp) : Bech32Address(chainHrp, Hash::HasherSha256ripemd, publicKey) {}

    /// Initializes an address with a public key and coin type.
    explicit Address(const PublicKey& publicKey, TWCoinType coin = TWCoinTypeBinance);

    static bool decode(const std::string& addr, Address& obj_out);
};

} // namespace TW::Binance
