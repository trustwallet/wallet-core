// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTezosAddress.h>

#include <TrustWalletCore/TWPublicKey.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>
#include "HexCoding.h"
#include "Tezos/TWTezosForger.h"

#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <tuple>


bool TWTezosAddressEqual(struct TWTezosAddress lhs, struct TWTezosAddress rhs) {
    return std::memcmp(lhs.bytes, rhs.bytes, TWTezosAddressSize) == 0;
}

bool TWTezosAddressIsValid(TWData *_Nonnull data) {
    return TWDataSize(data) == TWTezosAddressSize;
}


//

//
//
//

//
//
//

//
//
//
//bool TWTezosAddressIsValidString(TWString *_Nonnull string) {
//    auto str = TWStringUTF8Bytes(string);
//
//    size_t capacity = 128;
//    uint8_t buffer[capacity];
//
//    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
//    if (size != TWTezosAddressSize) {
//        return false;
//    }
//
//    return true;
//}

TWString *_Nullable TWTezosAddressForge() {
  auto input = "tz1Yju7jmmsaUiG9qQLoYv35v5pHgnWoLWbt";
  auto branch = "BL8euoCWqNCny9AR3AKjnpi38haYMxjei1ZqNHuXMn19JSQnoWp";
  auto result1 = forgeBranch(branch);
  auto result2 = forgeAddress(input);

  return nullptr;
}

// 3756ef37b1be849e3114643faa5847cabf9a896d3bfe4dd51448de68e91da1
// 3756ef37b1be849e3114643f0aa5847cabf9a896d3bfe4dd51448de68e91da0108000081faa75f741ef614b0e35fcc8c90dfa3b0b95721f80992
// f001f44e81020100008fb5cea62d147c696afd9a93dbce962f4c8a9c9100

bool TWTezosAddressInitWithString(struct TWTezosAddress *_Nonnull address, TWString *_Nonnull string) {
    auto str = TWStringUTF8Bytes(string);

    size_t capacity = 128;
    uint8_t buffer[capacity];

    int size = base58_decode_check(str, HASHER_SHA2D, buffer, (int)capacity);
    if (size != TWTezosAddressSize) {
        return false;
    }

    memcpy(address->bytes, buffer, TWTezosAddressSize);
    return true;
}

bool TWTezosAddressInitWithData(struct TWTezosAddress *_Nonnull address, TWData *_Nonnull data) {
    if (TWDataSize(data) != TWTezosAddressSize) {
        return false;
    }
    TWDataCopyBytes(data, 0, TWTezosAddressSize, address->bytes);
    return true;
}

bool TWTezosAddressInitWithPublicKey(struct TWTezosAddress *_Nonnull address, struct TWPublicKey publicKey, uint8_t prefix) {
    // Zcash taddr has two prefix bytes, the first byte is the same 0x1c -> t
    address->bytes[0] = 0x1c;
    address->bytes[1] = prefix;

    auto compressed = TWPublicKeyCompressed(publicKey);
    ecdsa_get_pubkeyhash(compressed.bytes, HASHER_SHA2_RIPEMD,  address->bytes + 2);

    return true;
}

TWString *_Nonnull TWTezosAddressDescription(struct TWTezosAddress address) {
    size_t size = 0;
    b58enc(nullptr, &size, address.bytes, TWTezosAddressSize);
    size += 16;

    std::vector<char> str(size);
    base58_encode_check(address.bytes, TWTezosAddressSize, HASHER_SHA2D, str.data(), size);

    return TWStringCreateWithUTF8Bytes(str.data());
}

TWData *_Nonnull TWTezosAddressData(struct TWTezosAddress address) {
    return TWDataCreateWithBytes(address.bytes, TWTezosAddressSize);
}
