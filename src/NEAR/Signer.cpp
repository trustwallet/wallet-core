// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../PrivateKey.h"

namespace TW::NEAR {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto transaction = transactionData(input);
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(transaction);
    auto signature = key.sign(hash, TWCurveED25519);
    auto output = Proto::SigningOutput();
    auto signedTransaction = signedTransactionData(transaction, signature);
    output.set_signed_transaction(signedTransaction.data(), signedTransaction.size());
    output.set_hash(hash.data(), hash.size());
    return output;
}

Data Signer::signaturePreimage() const {
    return TW::NEAR::transactionDataWithPublicKey(input);
};

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey) const {
    // validate public key
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key");
    }
    auto preImage = signaturePreimage();
    auto hash = Hash::sha256(preImage);
    {
        // validate correctness of signature
        if (!publicKey.verify(signature, hash)) {
            throw std::invalid_argument("Invalid signature/hash/publickey combination");
        }
    }
    auto signedPreImage = TW::NEAR::signedTransactionData(preImage, signature);
    auto output = Proto::SigningOutput();
    output.set_signed_transaction(signedPreImage.data(), signedPreImage.size());
    return output;
}
} // namespace TW::NEAR
