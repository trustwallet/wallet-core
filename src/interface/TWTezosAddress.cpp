// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Tezos/Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWTezosAddress.h>

using namespace TW;
using namespace TW::Tezos;

bool TWTezosAddressEqual(struct TWTezosAddress *_Nonnull lhs, struct TWTezosAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWTezosAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

TWString *_Nonnull TWTezosAddressDeriveOriginatedAddress(TWString *_Nonnull operationHash, int operationIndex) {
    auto s = reinterpret_cast<const std::string*>(operationHash);
    auto derivedAddress = Address::deriveOriginatedAddress(*s, operationIndex);
    return TWStringCreateWithUTF8Bytes(derivedAddress.c_str());
}

struct TWTezosAddress *_Nullable TWTezosAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWTezosAddress{ std::move(address) };
    } catch (...) {
        return nullptr;
    }
}

struct TWTezosAddress *_Nonnull TWTezosAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWTezosAddress{ Address(publicKey->impl) };
}

void TWTezosAddressDelete(struct TWTezosAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWTezosAddressKeyHash(struct TWTezosAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
}
