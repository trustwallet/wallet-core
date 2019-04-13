// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNULSAddress.h>

#include "../NULS/Address.h"

#include <memory>

using namespace TW::NULS;

/// Compares two addresses for equality.
bool TWNULSAddressEqual(struct TWNULSAddress* _Nonnull lhs, struct TWNULSAddress* _Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

/// Determines if the string is a valid NULS address.
bool TWNULSAddressIsValidString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

/// Creates an address from a string representaion.
struct TWNULSAddress* _Nullable TWNULSAddressCreateWithString(TWString* _Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    const auto address = Address(*s);
    return new TWNULSAddress{std::move(address)};
}

struct TWNULSAddress* _Nullable TWNULSAddressCreateWithData(TWData* _Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    return new TWNULSAddress{Address(*d)};
}

/// Creates an address from a public key.
struct TWNULSAddress* _Nonnull TWNULSAddressCreateWithPublicKey(
    struct TWPublicKey* _Nonnull publicKey) {
    return new TWNULSAddress{Address(publicKey->impl)};
}

/// Delete an address instance.
void TWNULSAddressDelete(struct TWNULSAddress* _Nonnull address) {
    delete address;
}

/// Returns the address string representation.
TWString* _Nonnull TWNULSAddressDescription(struct TWNULSAddress* _Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

/// Returns the key hash.
TWData* _Nonnull TWNULSAddressKeyHash(struct TWNULSAddress* _Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::size);
}
