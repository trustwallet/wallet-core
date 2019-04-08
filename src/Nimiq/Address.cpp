// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Hash.h"
#include "../HexCoding.h"

#include <TrezorCrypto/base32.h>
#include <TrezorCrypto/blake2b.h>

#include <algorithm>
#include <cassert>
#include <cmath>

using namespace TW::Nimiq;

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
    auto hash = std::array<uint8_t, Address::size>();
    if (base32_decode(string.data() + 4, 32, hash.data(), hash.size(), BASE32_ALPHABET_NIMIQ) == nullptr)
        return false;

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
    base32_decode(base32.data(), base32.length(), bytes.data(), bytes.size(),
                  BASE32_ALPHABET_NIMIQ);
}

Address::Address(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    auto hash = std::array<uint8_t, 32>();
    blake2b(publicKey.bytes.data() + 1, 32, hash.data(), hash.size());
    std::copy(hash.begin(), hash.begin() + Address::size, bytes.begin());
}

std::string Address::string() const {
    // Identifier code + blank checksum
    std::string string = "NQ00";
    // Checksum
    int check = 0;

    // Calculate Base32 sum
    auto base32 = std::array<uint8_t, 33>();
    base32_encode(bytes.begin(), Address::size, reinterpret_cast<char*>(base32.data()), 33,
                  BASE32_ALPHABET_NIMIQ);

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
    string[2] = '0' + (check / 10);
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
