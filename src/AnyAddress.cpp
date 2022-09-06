// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AnyAddress.h"

#include <utility>
#include "Coin.h"

namespace TW {

AnyAddress::AnyAddress(std::string address, enum TWCoinType coin) : address(std::move(address)), coin(coin) {

}

Data AnyAddress::getData() const {
    Data data;
    try {
        data = TW::addressToData(coin, address);
    } catch (...) {}
    return data;
}

AnyAddress* createAddress(const std::string& address, enum TWCoinType coin) {
    auto normalized = TW::normalizeAddress(coin, address);
    if (normalized.empty()) {
        return nullptr;
    }

    return new AnyAddress(normalized, coin);
}

AnyAddress* createAddress(const PublicKey& publicKey, enum TWCoinType coin) {
    auto derivedAddress = TW::deriveAddress(coin, publicKey);
    return new AnyAddress(derivedAddress, coin);
}

} // namespace TW
