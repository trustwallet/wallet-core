// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Base58.h"
#include "../PublicKey.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWZcashTAddress.h>

#include <cstring>
#include <string>
#include <vector>

using TW::PublicKey;

bool TWZcashTAddressEqual(struct TWZcashTAddress lhs, struct TWZcashTAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, TWZcashTAddressSize) == 0;
}

bool TWZcashTAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == TWZcashTAddressSize;
}

bool TWZcashTAddressIsValidString(TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);

    const auto decoded = TW::Base58::bitcoin.decodeCheck(s);
    if (decoded.size() != TWZcashTAddressSize) {
        return false;
    }

    return true;
}

bool TWZcashTAddressInitWithString(struct TWZcashTAddress *_Nonnull address, TWString *_Nonnull string) {
    auto& s = *reinterpret_cast<const std::string*>(string);

    const auto decoded = TW::Base58::bitcoin.decodeCheck(s);
    if (decoded.size() != TWZcashTAddressSize) {
        return false;
    }

    std::copy(decoded.begin(), decoded.end(), address->bytes);
    return true;
}

bool TWZcashTAddressInitWithData(struct TWZcashTAddress *_Nonnull address, TWData *_Nonnull data) {
    if (TWDataSize(data) != TWZcashTAddressSize) {
        return false;
    }
    TWDataCopyBytes(data, 0, TWZcashTAddressSize, address->bytes);
    return true;
}

bool TWZcashTAddressInitWithPublicKey(struct TWZcashTAddress *_Nonnull address, struct TWPublicKey *_Nonnull publicKey, uint8_t prefix) {
    // Zcash taddr has two prefix bytes, the first byte is the same 0x1c -> t
    address->bytes[0] = 0x1c;
    address->bytes[1] = prefix;

    auto compressed = publicKey->impl.compressed();
    ecdsa_get_pubkeyhash(compressed.bytes.data(), HASHER_SHA2_RIPEMD,  address->bytes + 2);

    return true;
}

TWString *_Nonnull TWZcashTAddressDescription(struct TWZcashTAddress address) {
    const auto str = TW::Base58::bitcoin.encodeCheck(address.bytes, address.bytes + TWZcashTAddressSize);
    return TWStringCreateWithUTF8Bytes(str.data());
}
