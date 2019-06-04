// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Waves/Address.h"

#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWWavesAddress.h>

#include <memory>

using namespace TW;
using namespace TW::Waves;

bool TWWavesAddressEqual(struct TWWavesAddress *_Nonnull lhs, struct TWWavesAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWWavesAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    return Address::isValid(*s);
}

struct TWWavesAddress *_Nullable TWWavesAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string *>(string);
    try {
        const auto address = Address(*s);
        return new TWWavesAddress{std::move(address)};
    } catch (...) {
        return nullptr;
    }
}

struct TWWavesAddress *_Nonnull TWWavesAddressCreateWithPublicKey(
    struct TWPublicKey *_Nonnull publicKey) {
    return new TWWavesAddress{Address(publicKey->impl)};
}

void TWWavesAddressDelete(struct TWWavesAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWWavesAddressDescription(struct TWWavesAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWWavesAddressKeyHash(struct TWWavesAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
}
