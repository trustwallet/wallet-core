// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#pragma once
#include "../uint256.h"
#include "../BinaryCoding.h"
#include "../proto/NULS.pb.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::NULS;


static inline void serializerRemark(std::string& remark, std::vector<uint8_t>& data) {
    encodeVarInt(remark.length(), data);
    std::copy(remark.begin(), remark.end(), std::back_inserter(data));
}

static inline void serializerInput(std::vector<Proto::TransactionCoinFrom>& inputs,
    std::vector<uint8_t>& data) {
    encodeVarInt(1, data);  //there is one coinFrom
    for (const auto& input : inputs) {
        const auto& fromAddress = input.from_address();
        if (!NULS::Address::isValid(fromAddress)) {
            throw std::invalid_argument("Invalid address");
        }
        const auto& addr = NULS::Address(fromAddress);
        encodeVarInt(addr.bytes.size() - 1, data);
        std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(data));
        encode16LE((uint16_t)input.assets_chainid(), data);
        encode16LE((uint16_t)input.idassets_id(), data);
        std::copy(input.idamount().begin(), input.idamount().end(), std::back_inserter(data));
        encodeVarInt(input.nonce().size(), data);
        std::copy(input.nonce().begin(), input.nonce().end(), std::back_inserter(data));
        data.push_back(static_cast<uint8_t>(input.locked()));
        break;
    }
}

static inline void serializerOutput(std::vector<Proto::TransactionCoinTo>& outputs,
    std::vector<uint8_t>& data) {
    encodeVarInt(1, data); //there is one coinTo
    for (const auto& output : outputs) {
        const auto& toAddress = output.to_address();
        if (!NULS::Address::isValid(toAddress)) {
            throw std::invalid_argument("Invalid address");
        }
        const auto& addr = NULS::Address(toAddress);
        encodeVarInt(addr.bytes.size() - 1, data);
        std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(data));
        encode16LE((uint16_t)output.assets_chainid(), data);
        encode16LE((uint16_t)output.idassets_id(), data);
        std::copy(output.idamount().begin(), output.idamount().end(), std::back_inserter(data));
        encode64LE(output.lock_time(), data);
        break;
    }
}

static inline Data calcTransactionDigest(Data& data) {
    Data hash1 = Hash::sha256(data);
    Data hash2 = Hash::sha256(hash1);
    return hash2;
}

static inline Data makeTransactionSignature(PrivateKey& privateKey, Data& txHash) {
    PublicKey pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    Data transactionSignature = Data();
    encodeVarInt(pubKey.bytes.size(), transactionSignature);
    std::copy(pubKey.bytes.begin(), pubKey.bytes.end(), std::back_inserter(transactionSignature));
    auto signature = privateKey.signAsDER(txHash, TWCurve::TWCurveSECP256k1);
    transactionSignature.push_back(static_cast<uint8_t>(0x00));
    encodeVarInt(signature.size(), transactionSignature);
    std::copy(signature.begin(), signature.end(), std::back_inserter(transactionSignature));
    return transactionSignature;
}
inline void encode256LE(Data &data, const uint256_t &value, uint32_t digit) {
    Data bytes = store(value);
    Data buff(digit / 8);

    for (int i = 0; i < (int)bytes.size(); ++i) {
        //int start = (int)buff.size() - (int)bytes.size() + i;
        //if (start >= 0) {
            buff[i] = bytes[i];
        //}
    }
    data.insert(data.end(), buff.begin(), buff.end());
}