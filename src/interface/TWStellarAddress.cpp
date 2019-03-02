// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWStellarAddress.h>

#include "../Stellar/Address.h"

#include <TrustWalletCore/TWPublicKey.h>

#include <string.h>
#include <memory>

using namespace TW;
using namespace TW::Stellar;

bool TWStellarAddressEqual(struct TWStellarAddress *_Nonnull lhs, struct TWStellarAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWStellarAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWStellarAddress *_Nullable TWStellarAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    const auto address = Address(*s);
    return new TWStellarAddress{ std::move(address) };
}

struct TWStellarAddress *_Nullable TWStellarAddressCreateWithData(TWData *_Nonnull data) {
    auto d = reinterpret_cast<const std::vector<uint8_t>*>(data);
    return new TWStellarAddress{ Address(*d) };
}

struct TWStellarAddress *_Nonnull TWStellarAddressCreateWithPublicKey(struct TWPublicKey publicKey) {
    std::vector<uint8_t> data;
    if (TWPublicKeyIsCompressed(publicKey)) {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::compressedSize);
    } else {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::uncompressedSize);
    }
    const auto address = Address(PublicKey(data));
    return new TWStellarAddress{ std::move(address) };
}

void TWStellarAddressDelete(struct TWStellarAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWStellarAddressDescription(struct TWStellarAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWStellarAddressKeyHash(struct TWStellarAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), Address::STELLAR_KEY_SIZE);
}
