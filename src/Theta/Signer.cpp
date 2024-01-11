// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
    auto encoded = transaction.encodePayload();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(signature.data(), signature.size());
    return output;
}

Data Signer::encode(const Transaction& transaction) const {
    const uint64_t nonce = 0;
    const uint64_t gasPrice = 0;
    const uint64_t gasLimit = 0;
    const auto* to = "0x0000000000000000000000000000000000000000";
    const uint64_t amount = 0;
    auto txData = transaction.encode(chainID);

    EthereumRlp::Proto::EncodingInput encodingInput;
    auto* rlpList = encodingInput.mutable_item()->mutable_list();

    /// Need to add the following prefix to the tx signbytes to be compatible with
    /// the Ethereum tx format
    rlpList->add_items()->set_number_u64(nonce);
    rlpList->add_items()->set_number_u64(gasPrice);
    rlpList->add_items()->set_number_u64(gasLimit);
    rlpList->add_items()->set_address(to);
    rlpList->add_items()->set_number_u64(amount);
    rlpList->add_items()->set_data(txData.data(), txData.size());

    return RLP::encode(encodingInput);
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
    auto encoded = transaction.encodePayload();

    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_signature(signature.data(), signature.size());
    return protoOutput;
}
} // namespace TW::Theta
