// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Address.h"

#include <TrezorCrypto/ecdsa.h>

#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

using namespace TW::NULS;

const std::string Address::prefix("NULSd");

bool Address::isValid(const std::string& string) {
    if (string.empty()) {
        return false;
    }
    if (string.length() <=  prefix.length()) {
        return false;
    }

    std::string address = string.substr(prefix.length(), string.length() - prefix.length());
    Data decoded = TW::Base58::bitcoin.decode(address);
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
    // Main-Net chainID
    bytes[0] = MainNetID;
    bytes[1] = 0x00;
    // Address Type
    bytes[2] = addressType;

    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, bytes.begin() + 3);

    // Calc chechsum
    uint8_t checkSum = 0x00;
    for (int i = 0; i < 23; ++i) {
        checkSum ^= bytes[i];
    }
    bytes[23] = checkSum;
}

Address::Address(const std::string& string) {
    if (false == isValid(string)){
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

bool Address::isValid() const {
    if (chainID() != MainNetID) {
        return false;
    }
    if (type() != 0x01 && type() != 0x02) {
        return false;
    }

    uint8_t checkSum = 0x00;
    for (int i = 0; i < 23; ++i) {
        checkSum ^= bytes[i];
    }

    return bytes[23] == checkSum;
}

TW::PrivateKey Address::importHexPrivateKey(const std::string& hexPrivateKey) {
    Data privKey = parse_hex(hexPrivateKey);
    Data data = Data();
    switch (privKey.size()) {
    case 31: {
        data.push_back(static_cast<uint8_t>(0x00));
        std::copy(privKey.begin(), privKey.end(), std::back_inserter(data));
    } break;
    case 32: {
        std::copy(privKey.begin(), privKey.end(), std::back_inserter(data));
    } break;
    case 33: {
        if (privKey[0] != 0x00) {
            throw std::invalid_argument("Invalid private key");
        }
        std::copy(privKey.begin() + 1, privKey.end(), std::back_inserter(data));
    } break;
    default: {
        throw std::invalid_argument("Invalid private key");
    }
    }

    auto key = PrivateKey(data);
    return key;
}

std::string Address::string() const {
    std::string temp = prefix;
    return temp.append(TW::Base58::bitcoin.encode(bytes.begin(), bytes.end()));
}

