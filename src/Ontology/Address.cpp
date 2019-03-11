// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Hash.h"
#include "Address.h"
#include "HexCoding.h"

#include <stdexcept>

#include <TrezorCrypto/sha2.h>
#include <TrezorCrypto/base58.h>
#include <TrezorCrypto/ripemd160.h>

using namespace TW::Ontology;

Address::Address(const PublicKey &publicKey) {
    std::vector<uint8_t> builder(publicKey.bytes);
    builder.insert(builder.begin(), 0x21);
    builder.push_back(0xAC);
    auto data = toScriptHash(builder);
    std::copy(data.begin(), data.end(), zero.begin());
}


Address::Address(const std::string &b58Address) {
    size_t dataSz = 25;
    std::array<uint8_t, 25> data{};
    b58tobin(data.data(), &dataSz, b58Address.data());
    if (data[0] != version) {
        throw std::runtime_error("Invalid address version.");
    }
    std::vector<uint8_t> addressWithVer(size + 1);
    std::copy(data.begin(), data.begin() + size + 2, addressWithVer.begin());
    auto checksum = Hash::sha256(Hash::sha256(addressWithVer));
    auto count = 1;
    for (auto v:checksum) {
        if (count == 5) {
            break;
        }
        if (v != *(data.begin() + size + count)) {
            throw std::runtime_error("Invalid base58 encode address.");
        }
        count++;
    }
    std::copy(addressWithVer.begin() + 1, addressWithVer.end(), zero.begin());
}

Address::Address(const std::vector<uint8_t> &data) {
    if (!Address::isValid(data)) {
        throw std::runtime_error("Invalid vector");
    }
    std::copy(data.begin(), data.end(), zero.begin());
}

std::vector<uint8_t> Address::toScriptHash(std::vector<uint8_t> &data) {
    return Hash::ripemd(Hash::sha256(data));
}

bool Address::isValid(const std::string &string) {
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

std::string Address::hexString() const {
    std::vector<uint8_t> data(size);
    std::copy(zero.rbegin(), zero.rend(), data.begin());
    return hex(data);
}

std::string Address::b58String() const {
    std::vector<uint8_t> data(size + 1);
    data[0] = version;
    std::copy(zero.begin(), zero.end(), data.begin() + 1);
    auto checksum = Hash::sha256(Hash::sha256(data));

    for (auto it = checksum.begin(); it != checksum.begin() + 4; it++) {
        data.push_back(*it);
    }
    size_t b58StrSz = 35;
    std::string b58Str(b58StrSz, ' ');
    b58enc(&b58Str[0], &b58StrSz, data.data(), data.size());
    b58Str.pop_back();
    return b58Str;
}