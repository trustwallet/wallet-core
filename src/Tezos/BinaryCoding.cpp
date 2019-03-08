// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Data.h"
#include "../PublicKey.h"
#include "../HexCoding.h"

#include <string>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ecdsa.h>
#include <iostream>

using namespace TW;

// Decode the given base 58 string and drop the given prefix from decoded data.
// Returns output length which output data placed in the inparameter.
int base58CheckDecodePrefix(const std::string& input, size_t prefixLength, uint8_t *prefix, uint8_t *output) {
    size_t capacity = 128;
    uint8_t decodedInput[capacity];
    int decodedLength = base58_decode_check(input.data(), HASHER_SHA2D, decodedInput, (int)capacity);

    // Verify that the prefix was correct.
    for (int i = 0; i < prefixLength; i++)
        if (decodedInput[i] != prefix[i])
            throw std::invalid_argument( "Invalid Prefix" );

    // Drop the prefix from branch.
    int outputLength = decodedLength - prefixLength;
    std::copy(decodedInput + prefixLength, decodedInput + prefixLength + outputLength, output);

    return outputLength;
}

// Forge the given boolean into a hex encoded string.
std::string forgeBool(bool input) {
    return input ? "ff" : "00";
}

// Forge the given public key hash into a hex encoded string.
// Note: This function supports tz1, tz2 and tz3 addresses.
std::string forgePublicKeyHash(const std::string &publicKeyHash) {
    std::string result = "0";
    size_t prefixLength = 3;
    uint8_t prefix[] = {6, 161, 0};
    size_t capacity = 128;
    uint8_t decoded[capacity];

    // Adjust prefix based on tz1, tz2 or tz3.
    switch ((char) publicKeyHash[2]) {
        case '1':
            result += "0";
            prefix[2] = 159;
            break;
        case '2':
            result += "1";
            prefix[2] = 161;
            break;
        case '3':
            result += "2";
            prefix[2] = 164;
            break;
        default:
          throw std::invalid_argument( "Invalid Prefix" );
    }
    int decodedLength = base58CheckDecodePrefix(publicKeyHash, prefixLength, prefix, decoded);
    result += TW::hex(decoded, decoded + decodedLength);
    return result;
}

// Forge the given public key hash into a hex encoded string.
// Note: This function only supports tz1 and KT1 addresses.
std::string forgeAddress(const std::string address) {
    std::string result = "";
    if ((char) address[0] == 'K') {
        size_t prefixLength = 3;
        uint8_t prefix[3] = {2, 90, 121};
        size_t capacity = 128;
        uint8_t decoded[capacity];

        int decodedLength = base58CheckDecodePrefix(address, prefixLength, prefix, decoded);
        result += "01";
        result += TW::hex(decoded, decoded + decodedLength);
        result += "00";
    } else {
        // implicit address
        result += "00";
        result += forgePublicKeyHash(address);
    }
    return result;
}

PublicKey parsePublicKey(std::string publicKey) {
    size_t prefixLength = 4;
    uint8_t prefix[] = {13, 15, 37, 217};
    size_t capacity = 33;
    uint8_t decoded[capacity];
    decoded[0] = 1;
    int decodedLength = base58CheckDecodePrefix(publicKey, prefixLength, prefix, decoded + 1);

    if (decodedLength != 32)
        throw std::invalid_argument( "Invalid Public Key" );

    return PublicKey(Data(decoded, decoded + 33));
}

// Forge the given public key into a hex encoded string.
std::string forgePublicKey(PublicKey publicKey) {
    size_t prefixLength = 4;
    uint8_t prefix[] = {13, 15, 37, 217};
    auto data = Data({13, 15, 37, 217});
    auto pk = Data(publicKey.bytes.begin() + 1, publicKey.bytes.end());
    append(data, pk);

    size_t size = 0;
    b58enc(nullptr, &size, data.data(), 36);
    size += 16;

    std::string publicKey(size, ' ');
    base58_encode_check(data.data(), 36, HASHER_SHA2D, &str[0], size);

    size_t capacity = 128;
    uint8_t decoded[capacity];
    int decodedLength = base58CheckDecodePrefix(publicKey, prefixLength, prefix, decoded);

    return "00" + TW::hex(decoded, decoded + decodedLength);
}
