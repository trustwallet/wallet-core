// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "../Data.h"
#include "../HexCoding.h"

using namespace TW;

// Forge the given boolean into a hex encoded string.
Data forgeBool(bool input) {
    auto result = input ? "ff" : "00";
    return parse_hex(result);
}

// Forge the given public key hash into a hex encoded string.
// Note: This function supports tz1, tz2 and tz3 addresses.
Data forgePublicKeyHash(const std::string &publicKeyHash) {
    Data forged = Data();
    size_t prefixLength = 3;
    uint8_t prefix[] = {6, 161, 0};
    size_t capacity = 128;
    uint8_t decoded[capacity];

    // Adjust prefix based on tz1, tz2 or tz3.
    switch ((char) publicKeyHash[2]) {
        case '1':
            append(forged, parse_hex("0"));
            prefix[2] = 159;
            break;
        case '2':
            append(forged, parse_hex("1"));
            prefix[2] = 161;
            break;
        case '3':
            append(forged, parse_hex("2"));
            prefix[2] = 164;
            break;
        default:
          throw std::invalid_argument( "Invalid Prefix" );
    }
    int decodedLength = base58CheckDecodePrefix(publicKeyHash, prefixLength, prefix, decoded);
    append(forged, parse_hex(hex(decoded, decoded + decodedLength)));
    return forged;
}

// Forge the given public key into a hex encoded string.
std::string forgePublicKey(PublicKey publicKey) {
    uint8_t prefix[] = {13, 15, 37, 217};
    auto data = Data(prefix, prefix + 4);
    auto bytes = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    append(data, bytes);

    auto pk = bytesToBase58(data.data(), data.size());
    auto decoded = base58ToHex(pk, 4, prefix);
    return decoded;
}
