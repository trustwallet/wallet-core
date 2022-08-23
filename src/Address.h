// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWData.h>
#include <string>

namespace TW {

class Address {
public:
    std::string address;

    enum TWCoinType coin;

    Address(const std::string& address, enum TWCoinType coin);

    Address(TWPublicKey* publicKey, enum TWCoinType coin);

    TWData* getData() const;
};

inline bool operator==(const Address& lhs, const Address& rhs) {
    return lhs.address == rhs.address && lhs.coin == rhs.coin;
}

inline bool operator!=(const Address& lhs, const Address& rhs) {
    return lhs.address != rhs.address || lhs.coin != rhs.coin;
}

} // namespace TW

/// Wrapper for C interface.
struct TWAnyAddress {
    TW::Address impl;
};
