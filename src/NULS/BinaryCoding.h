// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../uint256.h"
#include "../BinaryCoding.h"
#include "../proto/NULS.pb.h"
#include "../HexCoding.h"
#include "Address.h"

using namespace TW;
using namespace TW::NULS;

static inline void serializerRemark(std::string& remark, Data& data) {
    encodeVarInt(remark.length(), data);
    std::copy(remark.begin(), remark.end(), std::back_inserter(data));
}

static inline void serializerInput(const Proto::TransactionCoinFrom& input, Data& data) {
    encodeVarInt(1, data);  //there is one coinFrom
    const auto& fromAddress = input.from_address();
    if (!NULS::Address::isValid(fromAddress)) {
        throw std::invalid_argument("Invalid address");
    }
    const auto& addr = NULS::Address(fromAddress);
    encodeVarInt(addr.bytes.size() - 1, data);
    std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(data));
    encode16LE(static_cast<uint16_t>(input.assets_chainid()), data);
    encode16LE(static_cast<uint16_t>(input.assets_id()), data);
    std::copy(input.id_amount().begin(), input.id_amount().end(), std::back_inserter(data));
    Data nonce = parse_hex(input.nonce());
    encodeVarInt(nonce.size(), data);
    append(data, nonce);
    data.push_back(static_cast<uint8_t>(input.locked()));
}

static inline void serializerOutput(const Proto::TransactionCoinTo& output, Data& data) {
    encodeVarInt(1, data); //there is one coinTo

    const auto& toAddress = output.to_address();
    if (!NULS::Address::isValid(toAddress)) {
        throw std::invalid_argument("Invalid address");
    }
    const auto& addr = NULS::Address(toAddress);
    encodeVarInt(addr.bytes.size() - 1, data);
    std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(data));
    encode16LE(static_cast<uint16_t>(output.assets_chainid()), data);
    encode16LE(static_cast<uint16_t>(output.assets_id()), data);
    std::copy(output.id_amount().begin(), output.id_amount().end(), std::back_inserter(data));
    encode64LE(output.lock_time(), data);
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
    encodeVarInt(signature.size(), transactionSignature);
    std::copy(signature.begin(), signature.end(), std::back_inserter(transactionSignature));
    return transactionSignature;
}


