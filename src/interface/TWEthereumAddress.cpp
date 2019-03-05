// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAddress.h>

#include "../Data.h"
#include "../Ethereum/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::Ethereum;

bool TWEthereumAddressEqual(struct TWEthereumAddress *_Nonnull lhs, struct TWEthereumAddress *_Nonnull rhs) {
   return lhs->impl == rhs->impl;
}

bool TWEthereumAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWEthereumAddress *_Nullable TWEthereumAddressCreateWithString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    if (!Address::isValid(*s)) {
        return nullptr;
    }
    return new TWEthereumAddress{ Address(*s) };
}

struct TWEthereumAddress *_Nullable TWEthereumAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
    auto d = reinterpret_cast<const Data*>(keyHash);
    if (!Address::isValid(*d)) {
        return nullptr;
    }
    return new TWEthereumAddress{ Address(*d) };
}

struct TWEthereumAddress *_Nonnull TWEthereumAddressCreateWithPublicKey(struct TWPublicKey publicKey) {
    Data data;
    if (TWPublicKeyIsCompressed(publicKey)) {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::secp256k1Size);
    } else {
        data.insert(data.end(), publicKey.bytes, publicKey.bytes + PublicKey::secp256k1ExtendedSize);
    }
    return new TWEthereumAddress{ Address(PublicKey(data)) };
}

void TWEthereumAddressDelete(struct TWEthereumAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWEthereumAddressDescription(struct TWEthereumAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWEthereumAddressKeyHash(struct TWEthereumAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
