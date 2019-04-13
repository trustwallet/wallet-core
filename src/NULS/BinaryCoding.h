// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../BinaryCoding.h"
#include "../proto/NULS.pb.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::NULS;

/// Encodes a 48-bit little-endian value into the provided buffer.
static inline void encode48LE(uint64_t val, std::vector<uint8_t>& data) {
    data.push_back(static_cast<uint8_t>(val));
    data.push_back(static_cast<uint8_t>((val >> 8)));
    data.push_back(static_cast<uint8_t>((val >> 16)));
    data.push_back(static_cast<uint8_t>((val >> 24)));
    data.push_back(static_cast<uint8_t>((val >> 32)));
    data.push_back(static_cast<uint8_t>((val >> 40)));
}

static inline void serializerRemark(std::string& remark, std::vector<uint8_t>& data) {
    encodeVarInt(remark.length(), data);
    std::copy(remark.begin(), remark.end(), std::back_inserter(data));
}

static inline void serializerInput(std::vector<Proto::TransactionInput>& inputs,
    std::vector<uint8_t>& data) {
    encodeVarInt(inputs.size(), data);
    for (const auto& input : inputs) {
        Data owner = parse_hex(input.from_hash());
        // Input owner is txid and index
        encodeVarInt((uint16_t)input.from_index(), owner);

        encodeVarInt(owner.size(), data);
        std::copy(owner.begin(), owner.end(), std::back_inserter(data));

        encode64LE((uint64_t)input.amount(), data);
        encode48LE((uint64_t)input.lock_time(), data);
    }
}

static inline void serializerOutput(std::vector<Proto::TransactionOutput>& outputs,
    std::vector<uint8_t>& data) {
    encodeVarInt(outputs.size(), data);
    for (const auto& output : outputs) {
        const auto& toAddress = output.to_address();
        if (!NULS::Address::isValid(toAddress)) {
            throw std::invalid_argument("Invalid address");
        }

        const auto& addr = NULS::Address(toAddress);
        encodeVarInt(addr.bytes.size() - 1, data);
        std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(data));
        encode64LE((uint64_t)output.amount(), data);
        encode48LE((uint64_t)output.lock_time(), data);
    }
}

static inline Data calcTransactionDigest(Data& data) {
    Data hash1 = Hash::sha256(data);
    Data hash2 = Hash::sha256(hash1);
    return hash2;
}

static inline Data makeTransactionSignature(PrivateKey& privateKey, Data& txHash) {
    PublicKey pubKey = privateKey.getPublicKey(PublicKeyType::secp256k1);

    Data transactionSignature = Data();
    encodeVarInt(pubKey.bytes.size(), transactionSignature);
    std::copy(pubKey.bytes.begin(), pubKey.bytes.end(), std::back_inserter(transactionSignature));
    auto signature = privateKey.signAsDER(txHash, TWCurve::TWCurveSECP256k1);

    transactionSignature.push_back(static_cast<uint8_t>(0x00));
    encodeVarInt(signature.size(), transactionSignature);

    std::copy(signature.begin(), signature.end(), std::back_inserter(transactionSignature));
    return transactionSignature;
}