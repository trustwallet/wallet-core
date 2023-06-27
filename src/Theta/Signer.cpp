// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Ethereum/RLP.h"
#include "../Hash.h"

using RLP = TW::Ethereum::RLP;

namespace TW::Theta {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto pkFrom = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto from = Ethereum::Address(pkFrom.getPublicKey(TWPublicKeyTypeSECP256k1Extended));

    auto transaction = Transaction(
        /* from: */ from,
        /* to: */ Ethereum::Address(input.to_address()),
        /* thetaAmount: */ load(input.theta_amount()),
        /* tfuelAmount: */ load(input.tfuel_amount()),
        /* sequence: */ input.sequence(),
        /* feeAmount: */ load(input.fee()));

    auto signer = Signer(input.chain_id());
    auto signature = signer.sign(pkFrom, transaction);

    auto output = Proto::SigningOutput();

    transaction.setSignature(from, signature);
    auto encoded = transaction.encode();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(signature.data(), signature.size());
    return output;
}

Data Signer::encode(const Transaction& transaction) const {
    const uint64_t nonce = 0;
    const uint256_t gasPrice = 0;
    const uint64_t gasLimit = 0;
    const Ethereum::Address to = Ethereum::Address("0x0000000000000000000000000000000000000000");
    const uint256_t amount = 0;

    auto encoded = Data();
    /// Need to add the following prefix to the tx signbytes to be compatible with
    /// the Ethereum tx format
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    /// Chain ID
    auto payload = Data();
    append(payload, RLP::encode(chainID));
    append(payload, transaction.encode());
    append(encoded, RLP::encode(payload));
    return RLP::encodeList(encoded);
}

Data Signer::sign(const PrivateKey& privateKey, const Transaction& transaction) noexcept {
    auto encoded = encode(transaction);
    auto hash = Hash::keccak256(encoded);
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return signature;
}

Transaction Signer::buildTransaction() const{
    auto publicKey = PublicKey(Data(input.public_key().begin(), input.public_key().end()), TWPublicKeyTypeSECP256k1Extended);
    auto from = Ethereum::Address(publicKey);

    auto transaction = Transaction(
        /* from: */ from,
        /* to: */ Ethereum::Address(input.to_address()),
        /* thetaAmount: */ load(input.theta_amount()),
        /* tfuelAmount: */ load(input.tfuel_amount()),
        /* sequence: */ input.sequence(),
        /* feeAmount: */ load(input.fee()));
    return transaction;
}

Data Signer::signaturePreimage() const {
    return encode(this->buildTransaction());
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey) const{
    // validate public key
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Invalid public key");
    }
    {
        // validate correctness of signature
        auto preImage = signaturePreimage();
        auto preImageHash = Hash::keccak256(preImage);
        if (!publicKey.verify(signature, preImageHash)) {
            throw std::invalid_argument("Invalid signature/hash/publickey combination");
        }
    }
    auto from = Ethereum::Address(publicKey);
    auto protoOutput = Proto::SigningOutput();
    auto transaction = buildTransaction();
    transaction.setSignature(from, signature);
    auto encoded = transaction.encode();

    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_signature(signature.data(), signature.size());
    return protoOutput;
}
} // namespace TW::Theta
