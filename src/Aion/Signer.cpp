// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"

using namespace TW;

namespace TW::Aion {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Signer::buildTransaction(input);
    Signer::sign(key, transaction);

    auto output = Proto::SigningOutput();
    auto encoded = transaction.encode();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(transaction.signature.data(), transaction.signature.size());
    return output;
}

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    auto encoded = transaction.encode();
    auto hashData = Hash::blake2b(encoded, 32);
    auto hashSignature = privateKey.sign(hashData, TWCurveED25519);
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;

    // Aion signature = pubKeyBytes + signatureBytes
    Data result(publicKeyData.begin(), publicKeyData.end());
    result.insert(result.end(), hashSignature.begin(), hashSignature.end());

    transaction.signature = result;
}

TW::Data Signer::signaturePreimage(const Proto::SigningInput& input) noexcept {
    auto transaction = Signer::buildTransaction(input);
    auto encoded = transaction.encode();
    return transaction.encode();
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey, const Proto::SigningInput& input) noexcept {
    auto transaction = Signer::buildTransaction(input);

    // Aion signature = pubKeyBytes + signatureBytes
    Data result(publicKey.bytes.begin(), publicKey.bytes.end());
    result.insert(result.end(), signature.begin(), signature.end());

    transaction.signature = result;

    auto output = Proto::SigningOutput();
    auto encoded = transaction.encode();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(transaction.signature.data(), transaction.signature.size());

    return output;
}

Transaction Signer::buildTransaction(const Proto::SigningInput& input) noexcept {
    auto transaction = Transaction(
        /* nonce: */ static_cast<uint128_t>(load(input.nonce())),
        /* gasPrice: */ static_cast<uint128_t>(load(input.gas_price())),
        /* gasLimit: */ static_cast<uint128_t>(load(input.gas_limit())),
        /* to: */ Address(input.to_address()),
        /* amount: */ static_cast<uint128_t>(load(input.amount())),
        /* timestamp */ static_cast<uint128_t>(input.timestamp()),
        /* payload: */ Data(input.payload().begin(), input.payload().end()));

    return transaction;
}

} // namespace TW::Aion
