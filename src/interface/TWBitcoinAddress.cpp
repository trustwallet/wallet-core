// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "../Bitcoin/Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <cstring>

using namespace TW::Bitcoin;

bool TWBitcoinAddressEqual(struct TWBitcoinAddress lhs, struct TWBitcoinAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, Address::size) == 0;
}

bool TWBitcoinAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == Address::size;
}

bool TWBitcoinAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return Address::isValid(s);
}

bool TWBitcoinAddressInitWithString(struct TWBitcoinAddress *_Nonnull address, TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);

    const auto decoded = TW::Base58::bitcoin.decodeCheck(s);
    if (decoded.size() != Address::size) {
        return false;
    }

    std::copy(decoded.begin(), decoded.end(), address->bytes);
    return true;
}

bool TWBitcoinAddressInitWithData(struct TWBitcoinAddress *_Nonnull address, TWData *_Nonnull data) {
    if (TWDataSize(data) != Address::size) {
        return false;
    }
    TWDataCopyBytes(data, 0, Address::size, address->bytes);
    return true;
}

void TWBitcoinAddressInitWithPublicKey(struct TWBitcoinAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    address->bytes[0] = prefix;

    ecdsa_get_pubkeyhash(publicKey->impl.compressed().bytes.data(), HASHER_SHA2_RIPEMD,  address->bytes + 1);
}

TWString *_Nonnull TWBitcoinAddressDescription(struct TWBitcoinAddress address) {
    const auto str = TW::Base58::bitcoin.encodeCheck(address.bytes, address.bytes + Address::size);
    return TWStringCreateWithUTF8Bytes(str.data());
}

TWData *_Nonnull TWBitcoinAddressKeyhash(struct TWBitcoinAddress address) {
    return TWDataCreateWithBytes(address.bytes, Address::size);
}
