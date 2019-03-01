// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Ethereum/Signer.h"

using namespace TW;
using namespace TW::Wanchain;

void Signer::sign(const PrivateKey& privateKey, Ethereum::Transaction& transaction) const noexcept {
    auto hash = this->hash(transaction);
    auto tuple = Ethereum::Signer::sign(chainID, privateKey, hash);

    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data Signer::hash(const Ethereum::Transaction& transaction) const noexcept {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(1));
    append(encoded, Ethereum::RLP::encode(transaction.nonce));
    append(encoded, Ethereum::RLP::encode(transaction.gasPrice));
    append(encoded, Ethereum::RLP::encode(transaction.gasLimit));
    append(encoded, Ethereum::RLP::encode(transaction.to.bytes));
    append(encoded, Ethereum::RLP::encode(transaction.amount));
    append(encoded, Ethereum::RLP::encode(transaction.payload));
    append(encoded, Ethereum::RLP::encode(chainID));
    append(encoded, Ethereum::RLP::encode(0));
    append(encoded, Ethereum::RLP::encode(0));
    return Hash::keccak256(Ethereum::RLP::encodeList(encoded));
}
