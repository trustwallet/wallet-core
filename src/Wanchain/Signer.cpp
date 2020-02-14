// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Ethereum/Signer.h"

using namespace TW;
using namespace TW::Wanchain;

Ethereum::Proto::SigningOutput Signer::sign(const Ethereum::Proto::SigningInput& input) noexcept {
    auto signer = Signer(load(input.chain_id()));
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Ethereum::Signer::build(input);

    signer.sign(key, transaction);

    auto output = Ethereum::Proto::SigningOutput();

    auto encoded = signer.encode(transaction);
    output.set_encoded(encoded.data(), encoded.size());

    auto v = store(transaction.v);
    output.set_v(v.data(), v.size());

    auto r = store(transaction.r);
    output.set_r(r.data(), r.size());

    auto s = store(transaction.s);
    output.set_s(s.data(), s.size());

    return output;
}

void Signer::sign(const PrivateKey& privateKey, Ethereum::Transaction& transaction) const noexcept {
    transaction.v = chainID;
    transaction.r = 0;
    transaction.s = 0;
    auto hash = this->hash(transaction);
    auto tuple = Ethereum::Signer::sign(chainID, privateKey, hash);

    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);
}

Data Signer::encode(const Ethereum::Transaction& transaction) const noexcept {
    auto encoded = Data();
    append(encoded, Ethereum::RLP::encode(1));
    append(encoded, Ethereum::RLP::encode(transaction.nonce));
    append(encoded, Ethereum::RLP::encode(transaction.gasPrice));
    append(encoded, Ethereum::RLP::encode(transaction.gasLimit));
    append(encoded, Ethereum::RLP::encode(transaction.to));
    append(encoded, Ethereum::RLP::encode(transaction.amount));
    append(encoded, Ethereum::RLP::encode(transaction.payload));
    append(encoded, Ethereum::RLP::encode(transaction.v));
    append(encoded, Ethereum::RLP::encode(transaction.r));
    append(encoded, Ethereum::RLP::encode(transaction.s));
    return Ethereum::RLP::encodeList(encoded);
}

Data Signer::hash(const Ethereum::Transaction& transaction) const noexcept {
    const auto encoded = Signer::encode(transaction);
    return Hash::keccak256(encoded);
}
