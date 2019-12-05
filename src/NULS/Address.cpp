// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Address.h"

#include <TrezorCrypto/ecdsa.h>
#include <iostream>

#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::NULS;

std::string mainnetPrefix = std::string("NULSd");
std::string testnetPrefix = std::string("tNULSe");

bool Address::isValid(const std::string& addrStr) {
    if (addrStr.empty()) {
        return false;
    }
    std::string addrPrefix;
    if(addrStr.find(mainnetPrefix) == 0) {
        addrPrefix = mainnetPrefix;
    } else if (addrStr.find(testnetPrefix) == 0) {
        addrPrefix = testnetPrefix;
    } else {
        return false;
    }
    if (addrStr.length() <= addrPrefix.length()) {
        return false;
    }

    std::string address = addrStr.substr(addrPrefix.length(), addrStr.length() - addrPrefix.length());
    Data decoded = Base58::bitcoin.decode(address);
    if (decoded.size() != size) {
        return false;
    }

    // Check Xor
    uint8_t checkSum = 0x00;
    for (int i = 0; i < 23; ++i) {
        checkSum ^= decoded[i];
    }

    return decoded[23] == checkSum;
}

Address::Address(const TW::PublicKey& publicKey) {
    prefix = mainnetPrefix;
    // Main-Net chainID
    bytes[0] = 0x01;
    bytes[1] = 0x00;
    // Address Type
    bytes[2] = addressType;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.begin() + 3);
    bytes[23] = checksum(bytes);
}

Address::Address(const TW::PublicKey& publicKey, bool isMainnet) {
    if (isMainnet) {
        prefix = mainnetPrefix;
        bytes[0] = 0x01;
        bytes[1] = 0x00;
    } else {
        prefix = testnetPrefix;
        bytes[0] = 0x02;
        bytes[1] = 0x00;
    }
    // Address Type
    bytes[2] = addressType;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.begin() + 3);
    bytes[23] = checksum(bytes);
}

Address::Address(const std::string& string) {
    prefix = mainnetPrefix;
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    std::string address = string.substr(prefix.length(), string.length() - prefix.length()); 
    const auto decoded = Base58::bitcoin.decode(address);
    std::copy(decoded.begin(), decoded.end(), bytes.begin());
}

uint16_t Address::chainID() const {
    return decode16LE(bytes.data());
}

uint8_t Address::type() const {
    return bytes[2];
}

std::string Address::string() const {
    return prefix + Base58::bitcoin.encode(bytes.begin(), bytes.end());
}

uint8_t Address::checksum(std::array<byte, size>& byteArray) const{
    uint8_t checkSum = 0x00;
    for (int i = 0; i < 23; ++i) {
        checkSum ^= byteArray[i];
    }
    return checkSum;
}


