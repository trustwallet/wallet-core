// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "CashAddress.h"
#include "OpCodes.h"
#include "Script.h"
#include "SegwitAddress.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWHRP.h>

#include "../BinaryCoding.h"
#include "../Coin.h"
#include "../Decred/Address.h"
#include "../Groestlcoin/Address.h"
#include "../Zcash/TAddress.h"
#include "../Zen/Address.h"

#include <algorithm>
#include <iterator>
#include <cassert>

namespace TW::Bitcoin {

Data Script::hash() const {
    return Hash::ripemd(Hash::sha256(bytes));
}

bool Script::isPayToScriptHash() const {
    // Extra-fast test for pay-to-script-hash
    return bytes.size() == 23 && bytes[0] == OP_HASH160 && bytes[1] == 0x14 &&
           bytes[22] == OP_EQUAL;
}

bool Script::isPayToScriptHashReplay() const {
    // Extra-fast test for pay-to-script-hash-replay
    return bytes.size() == 61 && bytes[0] == OP_HASH160 && bytes[1] == 0x14 &&
           bytes[22] == OP_EQUAL && bytes.back() == OP_CHECKBLOCKATHEIGHT;
}

bool Script::isPayToWitnessScriptHash() const {
    // Extra-fast test for pay-to-witness-script-hash
    return bytes.size() == 34 && bytes[0] == OP_0 && bytes[1] == 0x20;
}

bool Script::isPayToWitnessPublicKeyHash() const {
    // Extra-fast test for pay-to-witness-public-key-hash
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

bool Script::matchPayToPublicKey(Data& result) const {
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

bool Script::matchPayToPublicKeyHash(Data& result) const {
    if (bytes.size() == 25 && bytes[0] == OP_DUP && bytes[1] == OP_HASH160 && bytes[2] == 20 &&
        bytes[23] == OP_EQUALVERIFY && bytes[24] == OP_CHECKSIG) {
        result.clear();
        std::copy(std::begin(bytes) + 3, std::begin(bytes) + 3 + 20, std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToPublicKeyHashReplay(Data& result) const {
    if (bytes.size() == 63 && bytes[0] == OP_DUP && bytes[1] == OP_HASH160 && bytes[2] == 20 &&
        bytes[23] == OP_EQUALVERIFY && bytes[24] == OP_CHECKSIG && bytes[25] == 32 &&
        bytes.back() == OP_CHECKBLOCKATHEIGHT) {
        result.clear();
        std::copy(std::begin(bytes) + 3, std::begin(bytes) + 3 + 20, std::back_inserter(result));
        return true;
    }
    return false;
}

bool Script::matchPayToScriptHash(Data& result) const {
    if (!isPayToScriptHash()) {
        return false;
    }
    result.clear();
    std::copy(std::begin(bytes) + 2, std::begin(bytes) + 22, std::back_inserter(result));
    return true;
}

bool Script::matchPayToScriptHashReplay(Data& result) const {
    if (!isPayToScriptHashReplay()) {
        return false;
    }
    result.clear();
    std::copy(std::begin(bytes) + 2, std::begin(bytes) + 22, std::back_inserter(result));
    return true;
}


bool Script::matchPayToWitnessPublicKeyHash(Data& result) const {
    if (!isPayToWitnessPublicKeyHash()) {
        return false;
    }
    result.clear();
    std::copy(std::begin(bytes) + 2, std::end(bytes), std::back_inserter(result));
    return true;
}

bool Script::matchPayToWitnessScriptHash(Data& result) const {
    if (!isPayToWitnessScriptHash()) {
        return false;
    }
    result.clear();
    std::copy(std::begin(bytes) + 2, std::end(bytes), std::back_inserter(result));
    return true;
}

bool Script::matchMultisig(std::vector<Data>& keys, int& required) const {
    keys.clear();
    required = 0;

    if (bytes.size() < 1 || bytes.back() != OP_CHECKMULTISIG) {
        return false;
    }

    size_t it = 0;
    uint8_t opcode;
    Data operand;

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
        if (!PublicKey::isValid(operand, TWPublicKeyTypeSECP256k1)) {
            break;
        }
        keys.push_back(operand);
    }

    if (!TWOpCodeIsSmallInteger(opcode)) {
        return false;
    }

    std::size_t expectedCount = decodeNumber(opcode);
    if (keys.size() != expectedCount || expectedCount < static_cast<std::size_t>(required)) {
        return false;
    }
    if (it + 1 != bytes.size()) {
        return false;
    }

    return true;
}

bool Script::getScriptOp(size_t& index, uint8_t& opcode, Data& operand) const {
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
        size = bytes[index];
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
    operand = Data(bytes.begin() + index, bytes.begin() + index + size);
    index += size;

    return true;
}

Script Script::buildPayToPublicKey(const Data& publicKey) {
    assert(publicKey.size() == PublicKey::secp256k1Size || publicKey.size() == PublicKey::secp256k1ExtendedSize);
    Script script;
    script.bytes.push_back(static_cast<byte>(publicKey.size()));
    append(script.bytes, publicKey);
    script.bytes.push_back(OP_CHECKSIG);
    return script;
}

Script Script::buildPayToPublicKeyHash(const Data& hash) {
    assert(hash.size() == 20);
    Script script;
    script.bytes.push_back(OP_DUP);
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    append(script.bytes, hash);
    script.bytes.push_back(OP_EQUALVERIFY);
    script.bytes.push_back(OP_CHECKSIG);
    return script;
}

Script Script::buildPayToPublicKeyHashReplay(const Data& hash, const Data& blockHash, int64_t blockHeight) {
    assert(hash.size() == 20);
    assert(blockHash.size() == 32);
    Script script;
    script.bytes.push_back(OP_DUP);
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    append(script.bytes, hash);
    script.bytes.push_back(OP_EQUALVERIFY);
    script.bytes.push_back(OP_CHECKSIG);

    // blockhash
    script.bytes.push_back(32);
    append(script.bytes, blockHash);

    // blockheight
    auto blockHeightData = encodeNumber(blockHeight);
    // blockHeight size will never beyond 1 byte size
    script.bytes.push_back(static_cast<byte>(blockHeightData.size()));
    append(script.bytes, blockHeightData);
    script.bytes.push_back(OP_CHECKBLOCKATHEIGHT);

    return script;
}


Script Script::buildPayToScriptHash(const Data& scriptHash) {
    assert(scriptHash.size() == 20);
    Script script;
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    append(script.bytes, scriptHash);
    script.bytes.push_back(OP_EQUAL);
    return script;
}

Script Script::buildPayToScriptHashReplay(const Data& scriptHash, const Data& blockHash, int64_t blockHeight) {
    assert(scriptHash.size() == 20);
    assert(blockHash.size() == 32);
    Script script;
    script.bytes.push_back(OP_HASH160);
    script.bytes.push_back(20);
    append(script.bytes, scriptHash);
    script.bytes.push_back(OP_EQUAL);

    // blockhash
    script.bytes.push_back(32);
    append(script.bytes, blockHash);

    // blockheight
    auto blockHeightData = encodeNumber(blockHeight);
    // blockHeight size will never beyond 1 byte size
    script.bytes.push_back(static_cast<byte>(blockHeightData.size()));
    append(script.bytes, blockHeightData);
    script.bytes.push_back(OP_CHECKBLOCKATHEIGHT);

    return script;
}


// Append to the buffer the length for the upcoming data (push). Supported length range: 0-75 bytes
void pushDataLength(Data& buffer, size_t len) {
    assert(len <= 255);
    if (len < static_cast<byte>(OP_PUSHDATA1)) {
        // up to 75 bytes, simple OP_PUSHBYTES with len
        buffer.push_back(static_cast<byte>(len));
        return;
    }
    // 75 < len < 256, OP_PUSHDATA with 1-byte len
    buffer.push_back(OP_PUSHDATA1);
    buffer.push_back(static_cast<byte>(len));
}

Script Script::buildPayToV0WitnessProgram(const Data& program) {
    assert(program.size() == 20 || program.size() == 32);
    Script script;
    script.bytes.push_back(OP_0);
    pushDataLength(script.bytes, static_cast<byte>(program.size()));
    append(script.bytes, program);
    assert(script.bytes.size() == 22 || script.bytes.size() == 34);
    return script;
}

Script Script::buildPayToWitnessPublicKeyHash(const Data& hash) {
    assert(hash.size() == 20);
    return Script::buildPayToV0WitnessProgram(hash);
}

Script Script::buildPayToWitnessScriptHash(const Data& scriptHash) {
    assert(scriptHash.size() == 32);
    return Script::buildPayToV0WitnessProgram(scriptHash);
}

Script Script::buildPayToV1WitnessProgram(const Data& publicKey) {
    assert(publicKey.size() == 32);
    Script script;
    script.bytes.push_back(OP_1);
    pushDataLength(script.bytes, static_cast<byte>(publicKey.size()));
    append(script.bytes, publicKey);
    assert(script.bytes.size() == 34);
    return script;
}

Script Script::buildOpReturnScript(const Data& data) {
    if (data.size() > MaxOpReturnLength) {
        // data too long, cannot fit, fail (do not truncate)
        return {};
    }
    assert(data.size() <= MaxOpReturnLength);
    Script script;
    script.bytes.push_back(OP_RETURN);
    pushDataLength(script.bytes, data.size());
    script.bytes.insert(script.bytes.end(), data.begin(), data.begin() + data.size());
    assert(script.bytes.size() <= 83); // max script length, must always hold
    return script;
}

void Script::encode(Data& data) const {
    encodeVarInt(bytes.size(), data);
    std::copy(std::begin(bytes), std::end(bytes), std::back_inserter(data));
}

Data Script::encodeNumber(int64_t n) {
    Data result;
    // check bitcoin Script::push_int64
    if (n == -1 || (n >= 1 && n <= 16)) {
        result.push_back(OP_1 + uint8_t(n - 1));
        return result;
    }
    if (n == 0) {
        result.push_back(OP_0);
        return result;
    }

    const bool neg = n < 0;
    uint64_t absvalue = neg ? -n : n;

    while (absvalue) {
        result.push_back(absvalue & 0xff);
        absvalue >>= 8;
    }

    if (result.back() & 0x80) {
        result.push_back(neg ? 0x80 : 0);
    } else if (neg) {
        result.back() |= 0x80;
    }
    return result;
}

bool isLtcP2sh(enum TWCoinType coin, byte start) {
    // For ltc, we need to support legacy p2sh which starts with 5.
    // Here we check prefix 5 and 50 in case of wallet-core changing its config value.
    // Ref: https://github.com/litecoin-project/litecoin/blob/0.21/src/chainparams.cpp#L128
    if (TWCoinTypeLitecoin == coin && (5 == start || 50 == start)) {
        return true;
    }
    return false;
}

Script Script::lockScriptForAddress(const std::string& string, enum TWCoinType coin) {
    // First try legacy address, for all coins
    if (Address::isValid(string)) {
        auto address = Address(string);
        auto p2pkh = TW::p2pkhPrefix(coin);
        auto p2sh = TW::p2shPrefix(coin);
        if (p2pkh == address.bytes[0]) {
            // address starts with 1/L
            auto data = Data();
            data.reserve(Address::size - 1);
            std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
            return buildPayToPublicKeyHash(data);
        } else if (p2sh == address.bytes[0]
            || isLtcP2sh(coin, address.bytes[0])) {
            // address starts with 3/M
            auto data = Data();
            data.reserve(Address::size - 1);
            std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
            return buildPayToScriptHash(data);
        }
        return {};
    }

    // Second, try Segwit address, for all coins; also check HRP
    if (const auto result = SegwitAddress::decode(string);
        std::get<2>(result) &&
        (std::get<1>(result) == stringForHRP(TW::hrp(coin)) || (coin == TWCoinTypeBitcoin && std::get<1>(result) == SegwitAddress::TestnetPrefix))
    ) {
        // address starts with bc/ltc/...
        const auto address = std::get<0>(result);
        if (address.witnessVersion == 0) {
            return buildPayToV0WitnessProgram(address.witnessProgram);
        }
        if (address.witnessVersion == 1 && address.witnessProgram.size() == 32) {
            return buildPayToV1WitnessProgram(address.witnessProgram);
        }
        return {};
    }

    // Thirdly, coin-specific address formats
    switch (coin) {
        case TWCoinTypeBitcoinCash:
            if (BitcoinCashAddress::isValid(string)) {
                auto address = BitcoinCashAddress(string);
                auto bitcoinAddress = address.legacyAddress();
                return lockScriptForAddress(bitcoinAddress.string(), TWCoinTypeBitcoinCash);
            }
            return {};

        case TWCoinTypeDecred:
            if (Decred::Address::isValid(string)) {
                auto bytes = Base58::decodeCheck(string, Rust::Base58Alphabet::Bitcoin, Hash::HasherBlake256d);
                if (bytes[1] == TW::p2pkhPrefix(TWCoinTypeDecred)) {
                    return buildPayToPublicKeyHash(Data(bytes.begin() + 2, bytes.end()));
                }
                if (bytes[1] == TW::p2shPrefix(TWCoinTypeDecred)) {
                    return buildPayToScriptHash(Data(bytes.begin() + 2, bytes.end()));
                }
            }
            return {};

        case TWCoinTypeECash:
            if (ECashAddress::isValid(string)) {
                auto address = ECashAddress(string);
                auto bitcoinAddress = address.legacyAddress();
                return lockScriptForAddress(bitcoinAddress.string(), TWCoinTypeECash);
            }
            return {};

        case TWCoinTypeGroestlcoin:
            if (Groestlcoin::Address::isValid(string)) {
                auto address = Groestlcoin::Address(string);
                auto data = Data();
                data.reserve(Address::size - 1);
                std::copy(address.bytes.begin() + 1, address.bytes.end(), std::back_inserter(data));
                if (address.bytes[0] == TW::p2pkhPrefix(TWCoinTypeGroestlcoin)) {
                    return buildPayToPublicKeyHash(data);
                }
                if (address.bytes[0] == TW::p2shPrefix(TWCoinTypeGroestlcoin)) {
                    return buildPayToScriptHash(data);
                }
            }
            return {};

        case TWCoinTypeZcash:
        case TWCoinTypeZelcash:
            if (Zcash::TAddress::isValid(string)) {
                auto address = Zcash::TAddress(string);
                auto data = Data();
                data.reserve(Address::size - 2);
                std::copy(address.bytes.begin() + 2, address.bytes.end(), std::back_inserter(data));
                if (address.bytes[1] == TW::p2pkhPrefix(TWCoinTypeZcash)) {
                    return buildPayToPublicKeyHash(data);
                } else if (address.bytes[1] == TW::p2shPrefix(TWCoinTypeZcash)) {
                    return buildPayToScriptHash(data);
                }
            }
            return {};

        default:
            return {};
    }
}

Script Script::lockScriptForAddress(const std::string& string, enum TWCoinType coin, const Data& blockHash, int64_t blockHeight) {
    if (Zen::Address::isValid(string)) {
        auto address = Zen::Address(string);
        auto data = Data();
        data.reserve(Address::size - 2);
        std::copy(address.bytes.begin() + 2, address.bytes.end(), std::back_inserter(data));
        if (address.bytes[1] == TW::p2pkhPrefix(TWCoinTypeZen)) {
            return buildPayToPublicKeyHashReplay(data, blockHash, blockHeight);
        } else if (address.bytes[1] == TW::p2shPrefix(TWCoinTypeZen)) {
            return buildPayToScriptHashReplay(data, blockHash, blockHeight);
        }
    }

    return lockScriptForAddress(string, coin);
}

Proto::TransactionOutput Script::buildBRC20InscribeTransfer(const std::string& ticker, uint64_t amount, const Data& publicKey) {
    TW::Bitcoin::Proto::TransactionOutput out;
    Rust::CByteArrayWrapper res = TW::Rust::tw_build_brc20_transfer_inscription(ticker.data(), amount, 0, publicKey.data(), publicKey.size());
    auto result = res.data;
    out.ParseFromArray(result.data(), static_cast<int>(result.size()));
    return out;
}

Proto::TransactionOutput Script::buildOrdinalNftInscription(const std::string& mimeType, const Data& payload, const Data& publicKey) {
    TW::Bitcoin::Proto::TransactionOutput out;
    Rust::CByteArrayWrapper res = TW::Rust::tw_bitcoin_build_nft_inscription(
        mimeType.data(),
        payload.data(),
        payload.size(),
        0,
        publicKey.data(),
        publicKey.size()
    );
    auto result = res.data;
    out.ParseFromArray(result.data(), static_cast<int>(result.size()));
    return out;
}

} // namespace TW::Bitcoin
