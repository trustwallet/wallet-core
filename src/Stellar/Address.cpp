// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <TrezorCrypto/base32.h>
#include <TrustWalletCore/TWStellarVersionByte.h>
#include <TrezorCrypto/memzero.h>

#include <cassert>
#include <HexCoding.h>
#include "Crc.h"
#include "Data.h"

using namespace TW::Stellar;

bool Address::isValid(const std::string& string) {
    bool valid = false;
    uint8_t decoded[rawSize];

    if (string.length() != size) {
        return false;
    }

    // Check that it decodes correctly
    uint8_t *ret = base32_decode(string.data(), size, decoded, sizeof(decoded), BASE32_ALPHABET_RFC4648);
    valid = (ret != NULL);

    // ... and that version byte is 0x30
    if (valid && TWStellarVersionByte(decoded[0]) != TWStellarVersionByte::TWStellarVersionByteAccountID) {
        valid = false;
    }

    // ... and that checksums match
    uint16_t checksum_expected = Crc::crc16(decoded, 33);
    uint16_t checksum_actual = (decoded[34] << 8) | decoded[33]; // unsigned short (little endian)
    if (valid && checksum_expected != checksum_actual) {
        valid = false;
    }

    memzero(decoded, sizeof(decoded));
    return valid;
}

Address::Address(const std::string& string) {
    uint8_t decoded[rawSize];
    // Ensure address is valid
    assert(isValid(string));

    base32_decode(string.data(), size, decoded, sizeof(decoded), BASE32_ALPHABET_RFC4648);
    std::copy(decoded + 1, decoded + 33, bytes.begin());
    memzero(decoded, sizeof(decoded));
}

Address::Address(const std::vector<uint8_t>& data) {
    assert(isValid(data));
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    auto publicKeyData = publicKey.bytes;
    std::copy(publicKeyData.begin() + 1, publicKeyData.begin() + 33, bytes.data());
}

std::string Address::string() const {
    char out[56 + 1];
    // version + key bytes + checksum
    uint8_t keylen = 1 + 32 + 2;
    uint8_t bytes_full[keylen];
    bytes_full[0] = 6 << 3; // 'G'

    std::copy(bytes.begin(), bytes.end(), bytes_full + 1);

    // Last two bytes are the checksum
    uint16_t checksum = Crc::crc16(bytes_full, 33);
    bytes_full[keylen-2] = checksum & 0x00ff;
    bytes_full[keylen-1] = (checksum>>8) & 0x00ff;

    base32_encode(bytes_full, keylen, out, sizeof(out), BASE32_ALPHABET_RFC4648);

    // Public key will always be 56 characters
    return std::string(out);
}