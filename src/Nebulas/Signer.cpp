// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

using namespace TW;
using namespace TW::Nebulas;

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {
    auto hash = this->hash(transaction);
    transaction.alg = 1;
    transaction.sign = privateKey.sign(hash, TWCurveSECP256k1);
}

Data Signer::toBigEndian(const uint256_t& value,uint32_t digit) {
    Data bytes;
    value.store(bytes);
    Data buff;
    buff.resize(digit / 8);

    for (int i = 0; i < (int)bytes.size(); ++i) {
        int start = (int)buff.size() - (int)bytes.size() + i;
        if (start >= 0) {
            buff[start] = bytes[i];
        }
    }
    return buff;
}

Data Signer::hash(const Transaction& transaction) const noexcept {
    auto encoded = Data();
    append(encoded, transaction.from.bytes);
    append(encoded, transaction.to.bytes);
    append(encoded, toBigEndian(transaction.amount,128));
    append(encoded, toBigEndian(transaction.nonce,64));
    append(encoded, toBigEndian(transaction.timestamp,64));
    append(encoded, transaction.payload);
    append(encoded, toBigEndian(chainID,32));
    append(encoded, toBigEndian(transaction.gasPrice,128));
    append(encoded, toBigEndian(transaction.gasLimit,128));
    return Hash::sha3_256(encoded);
}
