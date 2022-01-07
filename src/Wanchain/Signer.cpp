// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../Ethereum/Signer.h"

using namespace TW;
using namespace TW::Wanchain;

Ethereum::Proto::SigningOutput Signer::sign(const Ethereum::Proto::SigningInput &input) const noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Ethereum::Signer::build(input);

    auto signature = sign(key, transaction);

    auto protoOutput = Ethereum::Proto::SigningOutput();

    auto encoded = this->encode(transaction, signature);
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto v = store(signature.v);
    protoOutput.set_v(v.data(), v.size());

    auto r = store(signature.r);
    protoOutput.set_r(r.data(), r.size());

    auto s = store(signature.s);
    protoOutput.set_s(s.data(), s.size());

    return protoOutput;
}

Ethereum::Signature Signer::sign(const PrivateKey& privateKey, const std::shared_ptr<Ethereum::TransactionBase> transaction) const noexcept {
    auto emptySig = Ethereum::Signature{0, 0, chainID};
    auto txHash = this->hash(transaction, emptySig);
    return Ethereum::Signer::sign(chainID, privateKey, txHash, transaction->usesReplayProtection());
}

Data Signer::encode(const std::shared_ptr<Ethereum::TransactionBase> transaction,  const Ethereum::Signature &signature) const noexcept {
    std::shared_ptr<Ethereum::TransactionNonTyped> nonTypeTx = std::dynamic_pointer_cast<Ethereum::TransactionNonTyped>(transaction);
    if (nonTypeTx) {
        auto encoded = Data();
        append(encoded, Ethereum::RLP::encode(1));
        append(encoded, Ethereum::RLP::encode(nonTypeTx->nonce));
        append(encoded, Ethereum::RLP::encode(nonTypeTx->gasPrice));
        append(encoded, Ethereum::RLP::encode(nonTypeTx->gasLimit));
        append(encoded, Ethereum::RLP::encode(nonTypeTx->to));
        append(encoded, Ethereum::RLP::encode(nonTypeTx->amount));
        append(encoded, Ethereum::RLP::encode(nonTypeTx->payload));
        append(encoded, Ethereum::RLP::encode(signature.v));
        append(encoded, Ethereum::RLP::encode(signature.r));
        append(encoded, Ethereum::RLP::encode(signature.s));
        return Ethereum::RLP::encodeList(encoded);
    } else {
        return Data();
    }

}

Data Signer::hash(const std::shared_ptr<Ethereum::TransactionBase> transaction, const Ethereum::Signature &signature) const noexcept {
    const auto encoded = Signer::encode(transaction, signature);
    return Hash::keccak256(encoded);
}
