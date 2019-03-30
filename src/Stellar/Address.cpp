// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <TrezorCrypto/base32.h>
#include <TrezorCrypto/memzero.h>
#include <TrustWalletCore/TWStellarVersionByte.h>

#include "Crc.h"
#include "Data.h"
#include <HexCoding.h>
#include <cassert>

using namespace TW::Stellar;

bool Address::isValid(const std::string& string) {
    bool valid = false;
    std::array<uint8_t, rawSize> decoded;

    if (string.length() != size) {
        return false;
    }

    // Check that it decodes correctly
    uint8_t* ret = base32_decode(string.data(), size, decoded.data(), sizeof(decoded), BASE32_ALPHABET_RFC4648);
    valid = (ret != nullptr);

    // ... and that version byte is 0x30
    if (valid && TWStellarVersionByte(decoded[0]) != TWStellarVersionByte::TWStellarVersionByteAccountID) {
        valid = false;
    }

    // ... and that checksums match
    uint16_t checksum_expected = Crc::crc16(decoded.data(), 33);
    uint16_t checksum_actual = (decoded[34] << 8) | decoded[33]; // unsigned short (little endian)
    if (valid && checksum_expected != checksum_actual) {
        valid = false;
    }

    memzero(decoded.data(), sizeof(decoded));
    return valid;
}

Address::Address(const std::string& string) {
    // Ensure address is valid
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }

    std::array<uint8_t, rawSize> decoded;
    base32_decode(string.data(), size, decoded.data(), sizeof(decoded), BASE32_ALPHABET_RFC4648);
    std::copy(decoded.begin() + 1, decoded.end(), bytes.begin());
    memzero(decoded.data(), sizeof(decoded));
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    auto publicKeyData = publicKey.bytes;
    std::copy(publicKeyData.begin() + 1, publicKeyData.begin() + 33, bytes.data());
}

std::string Address::string() const {
    std::array<char, 56 + 1> out;
    // version + key bytes + checksum
    constexpr uint8_t keylen = 1 + 32 + 2;
    std::array<uint8_t, keylen> bytes_full;
    bytes_full[0] = 6 << 3; // 'G'

    std::copy(bytes.begin(), bytes.end(), bytes_full.begin() + 1);

    // Last two bytes are the checksum
    uint16_t checksum = Crc::crc16(bytes_full.data(), 33);
    bytes_full[keylen - 2] = checksum & 0x00ff;
    bytes_full[keylen - 1] = (checksum >> 8) & 0x00ff;

    base32_encode(bytes_full.data(), keylen, out.data(), sizeof(out), BASE32_ALPHABET_RFC4648);

    // Public key will always be 56 characters
    return std::string(out.data());
}