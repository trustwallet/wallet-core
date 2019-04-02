// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Serialization.h"

#include "../Hash.h"
#include "../HexCoding.h"
#include "../Base64.h"
#include "../PrivateKey.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <string>

using namespace TW;
using namespace TW::Cosmos;

std::vector<uint8_t> Signer::sign() const {
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(signaturePreimage());
    auto signature = key.sign(hash, TWCurveSECP256k1);
    return std::vector<uint8_t>(signature.begin(), signature.end() - 1);
}

std::string Signer::signInBase64() const {
    auto signature = sign();
    return Base64::encode(Data(signature.begin(), signature.end()));
}

std::string Signer::signaturePreimage() const {
    return transactionForSigningJSON(input).dump();
}

json Signer::buildTransactionJson(std::vector<uint8_t>& signature) const {    
    auto sig = Cosmos::Proto::Signature();
    sig.set_signature(signature.data(), signature.size());
    auto privateKey = PrivateKey(input.private_key());
    auto publicKey = privateKey.getPublicKey(PublicKeyType::secp256k1);
    sig.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto transaction = Cosmos::Proto::Transaction();
    *transaction.mutable_fee() = input.fee();
    transaction.set_memo(input.memo());
    *transaction.mutable_message() = input.message();
    *transaction.mutable_signature() = sig;

    return transactionJSON(transaction);
}

std::string Signer::buildTransaction() const {    
    auto signature = sign();
    return buildTransactionJson(signature).dump();
}