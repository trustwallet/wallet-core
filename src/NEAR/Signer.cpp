// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::NEAR {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto txResult = transactionData(input);
    if (!txResult) {
        Proto::SigningOutput output;
        output.set_error(txResult.error());
        output.set_error_message(Common::Proto::SigningError_Name(txResult.error()));
        return output;
    }
    auto transaction = txResult.payload();
    auto key = PrivateKey(input.private_key(), TWCurveED25519);
    auto hash = Hash::sha256(transaction);
    auto signature = key.sign(hash);
    auto output = Proto::SigningOutput();
    auto signedTransaction = signedTransactionData(transaction, signature);
    output.set_signed_transaction(signedTransaction.data(), signedTransaction.size());
    output.set_hash(hash.data(), hash.size());
    return output;
}

Result<Data, Common::Proto::SigningError> Signer::signaturePreimage() const {
    return TW::NEAR::transactionDataWithPublicKey(input);
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey) const {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key");
    }
    auto preImageResult = signaturePreimage();
    if (!preImageResult) {
        Proto::SigningOutput output;
        output.set_error(preImageResult.error());
        output.set_error_message(Common::Proto::SigningError_Name(preImageResult.error()));
        return output;
    }
    auto preImage = preImageResult.payload();
    auto hash = Hash::sha256(preImage);
    if (!publicKey.verify(signature, hash)) {
        Proto::SigningOutput output;
        output.set_error(Common::Proto::Error_signing);
        output.set_error_message("Invalid signature/hash/publickey combination");
        return output;
    }
    auto signedPreImage = TW::NEAR::signedTransactionData(preImage, signature);
    auto output = Proto::SigningOutput();
    output.set_signed_transaction(signedPreImage.data(), signedPreImage.size());
    return output;
}
} // namespace TW::NEAR
