// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Hash.h"

using namespace TW;

namespace TW::VeChain {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction();
    transaction.chainTag = static_cast<uint8_t>(input.chain_tag());
    transaction.blockRef = input.block_ref();
    transaction.expiration = input.expiration();
    for (auto& clause : input.clauses()) {
        transaction.clauses.emplace_back(clause);
    }
    transaction.gasPriceCoef = static_cast<uint8_t>(input.gas_price_coef());
    transaction.gas = input.gas();
    transaction.dependsOn = Data(input.depends_on().begin(), input.depends_on().end());
    transaction.nonce = input.nonce();
    transaction.signature = sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();

    auto encoded = transaction.encode();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_signature(transaction.signature.data(), transaction.signature.size());

    return protoOutput;
}

Data Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    auto encoded = transaction.encode();
    auto hash = Hash::blake2b(encoded, 32);
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return Data(signature.begin(), signature.end());
}

Data Signer::buildUnsignedTx(const Proto::SigningInput& input) noexcept {
    auto transaction = Transaction();
    transaction.chainTag = static_cast<uint8_t>(input.chain_tag());
    transaction.blockRef = input.block_ref();
    transaction.expiration = input.expiration();
    for (auto& clause : input.clauses()) {
        transaction.clauses.emplace_back(clause);
    }
    transaction.gasPriceCoef = static_cast<uint8_t>(input.gas_price_coef());
    transaction.gas = input.gas();
    transaction.dependsOn = Data(input.depends_on().begin(), input.depends_on().end());
    transaction.nonce = input.nonce();

    return transaction.encode();
}

Data Signer::buildSignedTx(const Proto::SigningInput& input, const Data& signature) noexcept {
    auto transaction = Transaction();
    transaction.chainTag = static_cast<uint8_t>(input.chain_tag());
    transaction.blockRef = input.block_ref();
    transaction.expiration = input.expiration();
    for (auto& clause : input.clauses()) {
        transaction.clauses.emplace_back(clause);
    }
    transaction.gasPriceCoef = static_cast<uint8_t>(input.gas_price_coef());
    transaction.gas = input.gas();
    transaction.dependsOn = Data(input.depends_on().begin(), input.depends_on().end());
    transaction.nonce = input.nonce();
    transaction.signature = signature;

    return transaction.encode();
}

} // namespace TW::VeChain
