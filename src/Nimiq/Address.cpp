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

static const char *BASE32_ALPHABET_NIMIQ = "0123456789ABCDEFGHJKLMNPQRSTUVXY";

static void check_append(std::string &, char);

bool Address::isValid(const std::string& stringPadded) {
    // Magic check
    if (stringPadded.substr(0, 2) != "NQ")
        return false;

    std::string string = stringPadded;

    // Remove spaces
    string.erase(
        std::remove(string.begin(), string.end(), ' '),
        string.end());

    if (string.length() != 36)
        return false;

    // Check if valid Base32
    auto hash = std::array<uint8_t, Address::size>();
    if (base32_decode(string.data() + 4, 32, hash.data(), hash.size(), BASE32_ALPHABET_NIMIQ) == NULL)
        return false;

    // Build checksum state
    std::string check = "";
    for (int i = 4; i < 36; i++)
        check_append(check, string[i]);
    check.append("232600");

    // Compress checksum state
    std::string tmp;
    for (int i = 0; i < (int)(std::ceil(check.length() / 6.0)); i++)
        tmp = std::to_string(std::stoi(tmp + check.substr(i * 6, 6)) % 97);
    int checksum_should = 98 - std::stoi(tmp);

    // Get checksum from input
    int checksum_is;
    try {
        checksum_is = std::stoi(string.substr(2, 2));
    } catch (const std::invalid_argument &ia) {
        return false;
    }

    if (checksum_is != checksum_should)
        return false;

    return true;
}

Address::Address(const std::string& stringPadded) {
    assert(isValid(stringPadded));

    std::string string = stringPadded;

    // Remove spaces
    string.erase(
        std::remove(string.begin(), string.end(), ' '),
        string.end());

    // Decode address
    auto base32 = string.substr(4, 32);
    base32_decode(base32.data(), base32.length(), bytes.data(), bytes.size(), BASE32_ALPHABET_NIMIQ);
}

Address::Address(const std::vector<uint8_t>& data) {
    assert(Address::isValid(data));
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const std::array<uint8_t, 32> publicKey) {
    auto hash = std::array<uint8_t, 32>();
    blake2b(publicKey.data(), 32, hash.data(), hash.size());
    std::copy(hash.begin(), hash.end() - Address::size, bytes.begin());
}

std::string Address::string() const {
    // Identifier code + blank checksum
    std::string string = "NQ00";
    // Checksum state representation of NQ00
    std::string check = "";

    // Calculate Base32 sum
    auto base32 = std::array<uint8_t, 33>();
    base32_encode(bytes.begin(), Address::size,
        reinterpret_cast<char *>(base32.data()), 33, BASE32_ALPHABET_NIMIQ);

    for (unsigned int i = 0; i < 32; i += 4) {
        // Add spaces to output
        string.append(" ");
        // Copy Base32 data
        string.append(base32.begin() + i, base32.begin() + i + 4);
        // Progress checksum state
        check_append(check, base32[i+0]);
        check_append(check, base32[i+1]);
        check_append(check, base32[i+2]);
        check_append(check, base32[i+3]);
    }
    check.append("232600");

    // Compress checksum state
    std::string tmp;
    for (int i = 0; i < (int)(std::ceil(check.length() / 6.0)); i++)
        tmp = std::to_string(std::stoi(tmp + check.substr(i * 6, 6)) % 97);

    // Finalize checksum
    char checksum[3];
    sprintf(checksum, "%02d", 98 - std::stoi(tmp));

    // Set checksum in address
    string[2] = checksum[0];
    string[3] = checksum[1];

    return string;
}

static void check_append(std::string &check, char c) {
    if (c >= 48 && c <= 57)
        check.push_back(c);
    else
        check.append(std::to_string(static_cast<int>(c - 55)));
}
