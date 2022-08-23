// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Coin.h"

namespace TW {

Address::Address(const std::string& address, enum TWCoinType coin) {
    auto normalized = TW::normalizeAddress(coin, address);
    if (normalized.empty()) {
        throw std::invalid_argument("unable to create address from string, not normalized");
    }

    this->address = normalized;
    this->coin = coin;
}

Address::Address(TWPublicKey* publicKey, enum TWCoinType coin) {
    auto derivedAddress = TW::deriveAddress(coin, publicKey->impl);

    this->address = derivedAddress;
    this->coin = coin;
}

TWData* Address::getData() const {
    Data data;
    try {
        data = TW::addressToData(coin, address);
    } catch (...) {}
    return TWDataCreateWithBytes(data.data(), data.size());
}

} // namespace TW
