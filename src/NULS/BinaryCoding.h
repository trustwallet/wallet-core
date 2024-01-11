// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"
#include "../proto/NULS.pb.h"
#include "../uint256.h"

using namespace TW;

namespace TW::NULS {

static inline void serializerRemark(std::string& remark, Data& data) {
    encodeVarInt(remark.length(), data);
    std::copy(remark.begin(), remark.end(), std::back_inserter(data));
}

static inline void serializerCoinData(const TW::NULS::Proto::Transaction& tx, Data& data) {
    auto coinData = Data();
    // CoinFrom
    encodeVarInt(tx.input_size(), coinData);
    for (int i = 0; i < tx.input_size(); i++) {
        // address
        const auto& fromAddress = tx.input(i).from_address();
        if (!NULS::Address::isValid(fromAddress)) {
            throw std::invalid_argument("Invalid address");
        }
        const auto& addr = NULS::Address(fromAddress);
        encodeVarInt(addr.bytes.size() - 1, coinData);
        std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(coinData));
        // chain id of asset
        encode16LE(static_cast<uint16_t>(tx.input(i).assets_chainid()), coinData);
        // asset id
        encode16LE(static_cast<uint16_t>(tx.input(i).assets_id()), coinData);
        // amount
        uint256_t numeric = load(tx.input(i).id_amount());
        Data numericData = store(numeric);
        std::reverse(numericData.begin(), numericData.end());
        std::string numericStr;
        numericStr.insert(numericStr.begin(), numericData.begin(), numericData.end());
        numericStr.append(static_cast<unsigned long>(numericData.capacity() - numericData.size()),
                          '\0');
        std::copy(numericStr.begin(), numericStr.end(), std::back_inserter(coinData));
        // nonce
        Data nonce = parse_hex(tx.input(i).nonce());
        encodeVarInt(nonce.size(), coinData);
        append(coinData, nonce);
        // locked
        coinData.push_back(static_cast<uint8_t>(tx.input(i).locked()));
    }
    encodeVarInt(tx.output_size(), coinData);
    for (int i = 0; i < tx.output_size(); i++) {
        // address
        const auto& toAddress = tx.output(i).to_address();
        if (!NULS::Address::isValid(toAddress)) {
            throw std::invalid_argument("Invalid address");
        }
        const auto& addr = NULS::Address(toAddress);
        encodeVarInt(addr.bytes.size() - 1, coinData);
        std::copy(addr.bytes.begin(), addr.bytes.end() - 1, std::back_inserter(coinData));
        // chain id of asset
        encode16LE(static_cast<uint16_t>(tx.output(i).assets_chainid()), coinData);
        // asset id
        encode16LE(static_cast<uint16_t>(tx.output(i).assets_id()), coinData);
        // amount
        uint256_t numeric = load(tx.output(i).id_amount());
        Data numericData = store(numeric);
        std::reverse(numericData.begin(), numericData.end());
        std::string numericStr;
        numericStr.insert(numericStr.begin(), numericData.begin(), numericData.end());
        numericStr.append(static_cast<unsigned long>(numericData.capacity() - numericData.size()),
                          '\0');
        std::copy(numericStr.begin(), numericStr.end(), std::back_inserter(coinData));
        // lock time
        encode64LE(tx.output(i).lock_time(), coinData);
    }
    encodeVarInt(tx.input_size() * Signer::TRANSACTION_INPUT_SIZE +
                     tx.output_size() * Signer::TRANSACTION_OUTPUT_SIZE,
                 data);
    append(data, coinData);
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
    auto signature = privateKey.signAsDER(txHash);
    encodeVarInt(signature.size(), transactionSignature);
    std::copy(signature.begin(), signature.end(), std::back_inserter(transactionSignature));
    return transactionSignature;
}

} // namespace TW::NULS
