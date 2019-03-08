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

using namespace TW;

// Decode the given base 58 string and drop the given prefix from decoded data.
// Returns output length which output data placed in the inparameter.
int base58CheckDecodePrefix(const std::string& input, size_t prefixLength, uint8_t *prefix, uint8_t *output) {
    size_t capacity = 128;
    uint8_t decoded[capacity];
    int decodedLength = base58_decode_check(input.data(), HASHER_SHA2D, decoded, (int)capacity);

    // Verify that the prefix was correct.
    for (int i = 0; i < prefixLength; i++)
        if (decoded[i] != prefix[i])
            throw std::invalid_argument( "Invalid Prefix" );

    std::copy(decoded + prefixLength, decoded + decodedLength, output);
    return decodedLength - prefixLength;
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

std::string bytesToBase58(const uint8_t *data, size_t dataSize) {
    size_t size = 0;
    b58enc(nullptr, &size, data, dataSize);
    size += 16;

    std::string encoded(size, ' ');
    base58_encode_check(data, dataSize, HASHER_SHA2D, &encoded[0], size);
    return std::string(encoded.c_str());
}

std::string base58ToHex(const std::string data, size_t prefixLength, uint8_t *prefix) {
    uint8_t decoded[128];
    int decodedLength = base58CheckDecodePrefix(data, prefixLength, prefix, decoded);
    return "00" + TW::hex(decoded, decoded + decodedLength);
}

PublicKey parsePublicKey(std::string publicKey) {
    size_t prefixLength = 4;
    uint8_t prefix[] = {13, 15, 37, 217};
    auto pk = Data({1});
    uint8_t decoded[32];
    int decodedLength = base58CheckDecodePrefix(publicKey, prefixLength, prefix, decoded);

    if (decodedLength != 32)
        throw std::invalid_argument( "Invalid Public Key" );
    append(pk, Data(decoded, decoded + 32));

    return PublicKey(pk);
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
