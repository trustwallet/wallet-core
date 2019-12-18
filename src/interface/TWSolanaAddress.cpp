// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Solana/Address.h"

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWSolanaAddress.h>

using namespace TW;
using namespace TW::Solana;

bool TWSolanaAddressEqual(struct TWSolanaAddress* _Nonnull lhs,
                          struct TWSolanaAddress* _Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWSolanaAddressIsValidString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWSolanaAddress* _Nullable TWSolanaAddressCreateWithString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    try {
        const auto address = Address(*s);
        return new TWSolanaAddress{std::move(address)};
    } catch (...) {
        return nullptr;
    }
}

struct TWSolanaAddress* _Nonnull TWSolanaAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey) {
    return new TWSolanaAddress{Address(publicKey->impl)};
}

void TWSolanaAddressDelete(struct TWSolanaAddress* _Nonnull address) {
    delete address;
}

TWString* _Nonnull TWSolanaAddressDescription(struct TWSolanaAddress* _Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}
