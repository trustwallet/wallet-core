// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Base64.h"
#include "PublicKey.h"

namespace {

enum IntentScope : int {
    TransactionData = 0,
};

enum IntentVersion : int {
    V0 = 0,
};

enum IntentAppId {
    Sui = 0
};

} // namespace

namespace TW::Sui {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto protoOutput = Proto::SigningOutput();
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    auto toSign = transactionPreimage(input);
    auto signature = privateKey.sign(TW::Hash::blake2b(toSign, 32), TWCurveED25519);
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    auto txSignatureScheme = signatureScheme(signature, publicKey);

    auto unsignedTx = input.sign_direct_message().unsigned_tx_msg();
    protoOutput.set_unsigned_tx(unsignedTx);
    protoOutput.set_signature(txSignatureScheme);
    return protoOutput;
}

TxCompiler::Proto::PreSigningOutput Signer::preImageHashes(const Proto::SigningInput& input) {
    TxCompiler::Proto::PreSigningOutput output;
    auto preImage = Signer::transactionPreimage(input);
    auto preImageHash = TW::Hash::blake2b(preImage, 32);
    output.set_data(preImage.data(), preImage.size());
    output.set_data_hash(preImageHash.data(), preImageHash.size());
    return output;
}

Data Signer::transactionPreimage(const Proto::SigningInput& input) {
    auto unsignedTx = input.sign_direct_message().unsigned_tx_msg();
    auto unsignedTxData = TW::Base64::decode(unsignedTx);
    Data toSign{TransactionData, V0, IntentAppId::Sui};
    append(toSign, unsignedTxData);
    return toSign;
}

std::string Signer::signatureScheme(const Data& signature, const PublicKey& publicKey) {
    Data signatureScheme{0x00};
    append(signatureScheme, signature);
    append(signatureScheme, publicKey.bytes);
    return TW::Base64::encode(signatureScheme);
}

// Data

} // namespace TW::Sui
