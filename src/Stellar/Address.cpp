// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

//#include "../HexCoding.h"

#include <TrezorCrypto/base32.h>

#include <cassert>
#include <TrezorCrypto/memzero.h>
#include <HexCoding.h>

using namespace TW::Stellar;

bool Address::isValid(const std::string& string) {
    bool valid = false;
    uint8_t decoded[STELLAR_ADDRESS_SIZE_RAW];

    if (string.length() != STELLAR_ADDRESS_SIZE) {
        return false;
    }

    // Check that it decodes correctly
    uint8_t *ret = base32_decode(string.data(), STELLAR_ADDRESS_SIZE, decoded, sizeof(decoded), BASE32_ALPHABET_RFC4648);
    valid = (ret != NULL);

    // ... and that version byte is 0x30
    if (valid && decoded[0] != 0x30) {
        valid = false;
    }

    // ... and that checksums match
    uint16_t checksum_expected = crc16(decoded, 33);
    uint16_t checksum_actual = (decoded[34] << 8) | decoded[33]; // unsigned short (little endian)
    if (valid && checksum_expected != checksum_actual) {
        valid = false;
    }

    memzero(decoded, sizeof(decoded));
    return valid;
}

Address::Address(const std::string& string) {
    uint8_t decoded[STELLAR_ADDRESS_SIZE_RAW];
    // Ensure address is valid
    assert(isValid(string));

    base32_decode(string.data(), STELLAR_ADDRESS_SIZE, decoded, sizeof(decoded), BASE32_ALPHABET_RFC4648);
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
    uint16_t checksum = crc16(bytes_full, 33);
    bytes_full[keylen-2] = checksum & 0x00ff;
    bytes_full[keylen-1] = (checksum>>8) & 0x00ff;

    base32_encode(bytes_full, keylen, out, sizeof(out), BASE32_ALPHABET_RFC4648);

    // Public key will always be 56 characters
    return std::string(out);
}

uint16_t Address::crc16(uint8_t *bytes, uint32_t length) {
    // Calculate checksum for existing bytes
    uint16_t crc = 0x0000;
    uint16_t polynomial = 0x1021;
    uint32_t i;
    uint8_t bit;
    uint8_t byte;
    uint8_t bitidx;
    uint8_t c15;

    for (auto i = 0; i < length; i++) {
        byte = bytes[i];
        for (bitidx=0; bitidx < 8; bitidx++) {
            bit = ((byte >> (7 - bitidx) & 1) == 1);
            c15 = ((crc >> 15 & 1) == 1);
            crc <<= 1;
            if (c15 ^ bit) {
                crc ^= polynomial;
            }
        }
    }

    return crc & 0xffff;
}
