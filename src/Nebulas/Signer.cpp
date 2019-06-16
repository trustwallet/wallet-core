// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::Nebulas;

const char* Signer::TxPayloadBinaryType = "binary";
const char* Signer::TxPayloadDeployType = "deploy";
const char* Signer::TxPayloadCallType = "call";

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {
    auto hash = this->hash(transaction);
    transaction.alg = 1;
    transaction.sign = privateKey.sign(hash, TWCurveSECP256k1);
}

void Signer::appendBigEndian(Data& data, const uint256_t& value,uint32_t digit) {
    Data bytes = store(value);
    Data buff(digit / 8);

    for (int i = 0; i < (int)bytes.size(); ++i) {
        int start = (int)buff.size() - (int)bytes.size() + i;
        if (start >= 0) {
            buff[start] = bytes[i];
        }
    }
    data.insert(data.end(),buff.begin(),buff.end());
}

Data Signer::hash(const Transaction& transaction) const noexcept {
    auto encoded = Data();
    encoded.insert(encoded.end(), transaction.from.bytes.begin(),transaction.from.bytes.end());
    encoded.insert(encoded.end(), transaction.to.bytes.begin(),transaction.to.bytes.end());
    appendBigEndian(encoded, transaction.amount,128);
    appendBigEndian(encoded, transaction.nonce,64);
    appendBigEndian(encoded, transaction.timestamp,64);
    encoded.insert(encoded.end(),transaction.payload.begin(),transaction.payload.end());
    appendBigEndian(encoded, chainID,32);
    appendBigEndian(encoded, transaction.gasPrice,128);
    appendBigEndian(encoded, transaction.gasLimit,128);
    return Hash::sha3_256(encoded);
}
