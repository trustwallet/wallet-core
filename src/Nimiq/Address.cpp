// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"

#include "../Base32.h"

#include <TrezorCrypto/blake2b.h>

#include <algorithm>

namespace TW::Nimiq {

static const char* BASE32_ALPHABET_NIMIQ = "0123456789ABCDEFGHJKLMNPQRSTUVXY";

static int check_append(int, uint8_t);
static inline int check_add(int, int);

bool Address::isValid(const std::string& stringPadded) {
    // Magic check
    if (stringPadded.substr(0, 2) != "NQ")
        return false;

    std::string string = stringPadded;

    // Remove spaces
    string.erase(std::remove(string.begin(), string.end(), ' '), string.end());

    if (string.length() != 36)
        return false;

    // Check if valid Base32
    Data hash;
    if (!Base32::decode(string.data() + 4, hash, BASE32_ALPHABET_NIMIQ)) {
        return false;
    }

    // Calculate checksum
    int check = 0;
    for (int i = 4; i < 36; i++)
        check = check_append(check, string[i]);
    check = check_add(check, 232600);
    check = 98 - check;

    // Get checksum from input
    int check_is;
    try {
        check_is = std::stoi(string.substr(2, 2));
    } catch (const std::invalid_argument& ia) {
        return false;
    }

    if (check_is != check)
        return false;

    return true;
}

Address::Address(const std::string& stringPadded) {
    if (!isValid(stringPadded)) {
        throw std::invalid_argument("Invalid address data");
    }

    std::string string = stringPadded;

    // Remove spaces
    string.erase(std::remove(string.begin(), string.end(), ' '), string.end());

    // Decode address
    auto base32 = string.substr(4, 32);
    Data data;
    if (!Base32::decode(base32, data, BASE32_ALPHABET_NIMIQ)) {
        throw std::invalid_argument("Invalid address data");
    }
    if (data.size() != size) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.data());
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    auto hash = std::array<uint8_t, 32>();
    blake2b(publicKey.bytes.data(), 32, hash.data(), hash.size());
    std::copy(hash.begin(), hash.begin() + Address::size, bytes.begin());
}

std::string Address::string() const {
    // Identifier code + blank checksum
    std::string string = "NQ00";
    // Checksum
    int check = 0;

    // Calculate Base32 sum
    Data bytesAsData;
    bytesAsData.assign(bytes.begin(), bytes.end());
    auto base32 = Base32::encode(bytesAsData, BASE32_ALPHABET_NIMIQ);

    for (auto i = 0; i < 32; i += 4) {
        // Add spaces to output
        string.append(" ");
        // Copy Base32 data
        string.append(base32.begin() + i, base32.begin() + i + 4);
        // Progress checksum state
        check = check_append(check, base32[i + 0]);
        check = check_append(check, base32[i + 1]);
        check = check_append(check, base32[i + 2]);
        check = check_append(check, base32[i + 3]);
    }

    // Finalize checksum
    check = check_add(check, 232600); // NQ00
    check = 98 - check;

    // Set checksum in address
    string[2] = '0' + static_cast<uint8_t>((check / 10));
    string[3] = '0' + (check % 10);

    return string;
}

static int check_append(int check, uint8_t c) {
    int num;
    if (c >= '0' && c <= '9')
        num = c - '0';
    else
        num = c - '7';
    return check_add(check, num);
}

static inline int check_add(int check, int num) {
    if (num == 0)
        return (check * 10) % 97;

    // check = check * 10^(log_10(num))
    for (int remainder = num; remainder > 0; check *= 10, remainder /= 10)
        ;
    return (check + num) % 97;
}

} // namespace TW::Nimiq
