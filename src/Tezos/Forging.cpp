// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "BinaryCoding.h"
#include "../Base58.h"
#include "../Data.h"
#include "../HexCoding.h"
#include "../BinaryCoding.h"

#include <sstream>

using namespace TW;

// Forge the given boolean into a hex encoded string.
Data forgeBool(bool input) {
    unsigned char result = input ? 0xff : 0x00;
    return Data { result };
}

// Forge the given public key hash into a hex encoded string.
// Note: This function supports tz1, tz2 and tz3 addresses.
Data forgePublicKeyHash(const std::string &publicKeyHash) {
    Data forged = Data();
    std::array<byte, 3> prefix = {2, 90, 121};

    // Adjust prefix based on tz1, tz2 or tz3.
    switch ((char) publicKeyHash[2]) {
        case '1':
            forged.push_back(0x00);
            prefix[2] = 159;
            break;
        case '2':
            forged.push_back(0x01);
            prefix[2] = 161;
            break;
        case '3':
            forged.push_back(0x02);
            prefix[2] = 164;
            break;
        default:
          throw std::invalid_argument( "Invalid Prefix" );
    }
    const auto decoded = Base58::bitcoin.decodeCheck(publicKeyHash);
    forged.insert(forged.end(), decoded.begin() + prefix.size(), decoded.end());
    return forged;
}

// Forge the given public key into a hex encoded string.
Data forgePublicKey(PublicKey publicKey) {
    uint8_t prefix[] = {13, 15, 37, 217};
    auto data = Data(prefix, prefix + 4);
    auto bytes = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    append(data, bytes);

    auto pk = Base58::bitcoin.encodeCheck(data);
    auto decoded = base58ToHex(pk, 4, prefix);
    return parse_hex(decoded);
}

// Forge the given zarith hash into a hex encoded string.
Data forgeZarith(uint16_t input) {
    Data forged = Data();
    while (true) {
        if (input < 128) {
            if (input < 16) {
                forged.push_back(0x0);
            }
            Data encoded;
            encode16BE(input, encoded);
            append(forged, encoded);
            break;
        } else {
//            uint64_t b = input % 128;
//            input -= b;
//            input /= 128;
//            b += 128;
//            Data encoded;
//            encode16BE(b, encoded);
//            append(forged, encoded);
        }
    }
    return forged;
}
