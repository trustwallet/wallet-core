// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Script.h"

#include "Address.h"
#include "Bech32Address.h"
#include "CashAddress.h"

#include "../BinaryCoding.h"
#include "../Decred/Address.h"
#include "../Groestlcoin/Address.h"
#include "../Hash.h"
#include "../PublicKey.h"
#include "../Zcash/TAddress.h"

#include <TrustWalletCore/TWBitcoinOpCodes.h>
#include <TrustWalletCore/TWP2PKHPrefix.h>
#include <TrustWalletCore/TWP2SHPrefix.h>

#include <algorithm>
#include <cassert>

using namespace TW::Bitcoin;

std::vector<uint8_t> Script::hash() const {
    return TW::Hash::ripemd(TW::Hash::sha256(bytes));
}

bool Script::isPayToScriptHash() const {
    // Extra-fast test for pay-to-script-hash
    return bytes.size() == 23 && bytes[0] == OP_HASH160 && bytes[1] == 0x14 &&
           bytes[22] == OP_EQUAL;
}

bool Script::isPayToWitnessScriptHash() const {
    // Extra-fast test for pay-to-witness-script-hash
    return bytes.size() == 22 && bytes[0] == OP_0 && bytes[1] == 0x14;
}

bool Script::isWitnessProgram() const {
    if (bytes.size() < 4 || bytes.size() > 42) {
        return false;
    }
    if (bytes[0] != OP_0 && (bytes[0] < OP_1 || bytes[0] > OP_16)) {
        return false;
    }
    return bytes[1] + 2 == bytes.size();
}

bool Script::matchPayToPubkey(std::vector<uint8_t>& result) const {
    if (bytes.size() == PublicKey::secp256k1ExtendedSize + 2 &&
        bytes[0] == PublicKey::secp256k1ExtendedSize && bytes.back() == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 1, std::begin(bytes) + 1 + PublicKey::secp256k1Size,
                  std::back_inserter(result));
        return true;
    }
    if (bytes.size() == PublicKey::secp256k1Size + 2 && bytes[0] == PublicKey::secp256k1Size &&
        bytes.back() == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 1, std::begin(bytes) + 1 + PublicKey::secp256k1Size,
                  std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToPubkeyHash(std::vector<uint8_t>& result) const {
    if (bytes.size() == 25 && bytes[0] == OP_DUP && bytes[1] == OP_HASH160 && bytes[2] == 20 &&
        bytes[23] == OP_EQUALVERIFY && bytes[24] == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 3, std::begin(bytes) + 3 + 20, std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToScriptHash(std::vector<uint8_t>& result) const {
    if (!isPayToScriptHash()) {
        return false;
    }
    result.clear();
    std::copy(std::begin(bytes) + 2, std::begin(bytes) + 22, std::back_inserter(result));
    return true;
}

bool Script::matchPayToWitnessPublicKeyHash(std::vector<uint8_t>& result) const {
    if (bytes.size() == 22 && bytes[0] == OP_0 && bytes[1] == 0x14) {
        result.clear();
        std::copy(std::begin(bytes) + 2, std::end(bytes), std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToWitnessScriptHash(std::vector<uint8_t>& result) const {
    if (bytes.size() == 34 && bytes[0] == OP_0 && bytes[1] == 0x20) {
        result.clear();
        std::copy(std::begin(bytes) + 2, std::end(bytes), std::back_inserter(result));
        return true;
    }
    return false;
}

/// Decodes a small integer
static inline int decodeNumber(uint8_t opcode) {
    if (opcode == OP_0) {
        return 0;
    }
    assert(opcode >= OP_1 && opcode <= OP_16);
    return static_cast<int>(opcode) - static_cast<int>(OP_1 - 1);
}

bool Script::matchMultisig(std::vector<std::vector<uint8_t>>& keys, int& required) const {
    keys.clear();
    required = 0;

    if (bytes.size() < 1 || bytes.back() != OP_CHECKMULTISIG) {
        return false;
    }

    size_t it = 0;
    uint8_t opcode;
    std::vector<uint8_t> operand;

    auto op = getScriptOp(it, opcode, operand);
    if (!op || !TWOpCodeIsSmallInteger(opcode)) {
        return false;
    }
    required = decodeNumber(opcode);
    while (true) {
        auto res = getScriptOp(it, opcode, operand);
        if (!res) {
            break;
        }
        if (!TW::PublicKey::isValid(operand)) {
            break;
        }
        keys.push_back(operand);
    }

    if (!TWOpCodeIsSmallInteger(opcode)) {
        return false;
    }

    auto expectedCount = decodeNumber(opcode);
    if (keys.size() != expectedCount || expectedCount < required) {
        return false;
    }
    if (it + 1 != bytes.size()) {
        return false;
    }

    return true;
}

bool Script::getScriptOp(size_t& index, uint8_t& opcode, std::vector<uint8_t>& operand) const {
    operand.clear();

    // Read instruction
    if (index >= bytes.size()) {
        return false;
    }

    opcode = bytes[index];
    index += 1;

    if (opcode > OP_PUSHDATA4) {
        return true;
    }

    // Immediate operand
    size_t size = 0;
    if (opcode < OP_PUSHDATA1) {
        size = static_cast<size_t>(opcode);
    } else if (opcode == OP_PUSHDATA1) {
        if (bytes.size() - index < 1) {
            return false;
        }
        size = index;
        index += 1;
    } else if (opcode == OP_PUSHDATA2) {
        if (bytes.size() - index < 2) {
            return false;
        }
        size = static_cast<size_t>(decode16LE(bytes.data() + index));
        index += 2;
    } else if (opcode == OP_PUSHDATA4) {
        if (bytes.size() - index < 4) {
            return false;
        }
        size = static_cast<size_t>(decode32LE(bytes.data() + index));
        index += 4;
    }
    if (bytes.size() - index < size) {
        return false;
    }
    operand = std::vector<uint8_t>(bytes.begin() + index, bytes.begin() + index + size);
    index += size;

    return true;
}

Script Script::buildPayToPublicKeyHash(const std::vector<uint8_t>& hash) {
    assert(hash.size() == 20);
    Script script;
    script.bytes.push_back(OP_DUP);
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    script.bytes.insert(script.bytes.end(), hash.begin(), hash.end());
    script.bytes.push_back(OP_EQUALVERIFY);
    script.bytes.push_back(OP_CHECKSIG);
    return script;
}

Script Script::buildPayToScriptHash(const std::vector<uint8_t>& scriptHash) {
    assert(scriptHash.size() == 20);
    Script script;
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    script.bytes.insert(script.bytes.end(), scriptHash.begin(), scriptHash.end());
    script.bytes.push_back(OP_EQUAL);
    return script;
}

Script Script::buildPayToWitnessPubkeyHash(const std::vector<uint8_t>& hash) {
    assert(hash.size() == 20);
    Script script;
    script.bytes.push_back(OP_0);
    script.bytes.push_back(20);
    script.bytes.insert(script.bytes.end(), hash.begin(), hash.end());
    return script;
}

Script Script::buildPayToWitnessScriptHash(const std::vector<uint8_t>& scriptHash) {
    assert(scriptHash.size() == 32);
    Script script;
    script.bytes.push_back(OP_0);
    script.bytes.push_back(32);
    script.bytes.insert(script.bytes.end(), scriptHash.begin(), scriptHash.end());
    return script;
}

void Script::encode(std::vector<uint8_t>& data) const {
    encodeVarInt(bytes.size(), data);
    std::copy(std::begin(bytes), std::end(bytes), std::back_inserter(data));
}

Script Script::buildForAddress(const std::string& string) {
    static const std::vector<uint8_t> p2pkhPrefixes = {TWP2PKHPrefixBitcoin, TWP2PKHPrefixLitecoin,
                                                       TWP2PKHPrefixDash, TWP2PKHPrefixZcoin, TWP2PKHPrefixViacoin,
                                                       TWP2PKHPrefixDogecoin, TWP2PKHPrefixQtum};
    static const std::vector<uint8_t> p2shPrefixes = {TWP2SHPrefixBitcoin, TWP2SHPrefixLitecoin,
                                                      TWP2SHPrefixDash, TWP2SHPrefixZcoin, TWP2SHPrefixViacoin,
                                                      TWP2SHPrefixDogecoin, TWP2SHPrefixLux};
    if (Address::isValid(string)) {
        auto address = Address(string);
        auto p2pkh = std::find(p2pkhPrefixes.begin(), p2pkhPrefixes.end(), address.bytes[0]);
        if (p2pkh != p2pkhPrefixes.end()) {
            // address starts with 1/L
            auto data = std::vector<uint8_t>();
            data.reserve(Address::size - 1);
            std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
            return buildPayToPublicKeyHash(data);
        }
        auto p2sh = std::find(p2shPrefixes.begin(), p2shPrefixes.end(), address.bytes[0]);
        if (p2sh != p2shPrefixes.end()) {
            // address starts with 3/M
            auto data = std::vector<uint8_t>();
            data.reserve(Address::size - 1);
            std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
            return buildPayToScriptHash(data);
        }
    } else if (Bech32Address::isValid(string)) {
        auto result = Bech32Address::decode(string);
        // address starts with bc/ltc
        auto program = result.first.witnessProgram;
        return buildPayToWitnessPubkeyHash(program);
    } else if (CashAddress::isValid(string)) {
        auto address = CashAddress(string);
        auto bitcoinAddress = address.legacyAddress();
        return buildForAddress(bitcoinAddress.string());
    } else if (Decred::Address::isValid(string)) {
        auto address = Decred::Address(string);
        return buildPayToPublicKeyHash(Data(address.keyhash.begin(), address.keyhash.end()));
    } else if (Groestlcoin::Address::isValid(string)) {
        auto address = Groestlcoin::Address(string);
        auto data = std::vector<uint8_t>();
        data.reserve(Address::size - 1);
        std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
        if (address.bytes[0] == TWP2PKHPrefixGroestlcoin) {
            return buildPayToPublicKeyHash(data);
        }
        if (address.bytes[0] == TWP2SHPrefixGroestlcoin) {
            return buildPayToScriptHash(data);
        }
    } else if (Zcash::TAddress::isValid(string)) {
        auto address = Zcash::TAddress(string);
        auto data = std::vector<uint8_t>();
        data.reserve(Address::size - 2);
        std::copy(address.bytes.begin() + 2, address.bytes.end(), std::back_inserter(data));
        return buildPayToPublicKeyHash(data);
    }
    return {};
}
