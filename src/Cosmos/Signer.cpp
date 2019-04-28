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
#include "../Data.h"

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <string>

using namespace TW;
using namespace TW::Cosmos;

using json = nlohmann::json;

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
    return signaturePreimageJSON(input).dump();
}

json Signer::buildTransactionJSON(const Data& signature) const {    
    auto sig = Cosmos::Proto::Signature();
    sig.set_signature(signature.data(), signature.size());
    auto privateKey = PrivateKey(input.private_key());
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    sig.set_public_key(publicKey.bytes.data(), publicKey.bytes.size());

    auto transaction = Cosmos::Proto::Transaction();
    *transaction.mutable_fee() = input.fee();
    transaction.set_memo(input.memo());

    if (input.has_send_coins_message()) {
        *transaction.mutable_send_coins_message() = input.send_coins_message();
    } else if (input.has_stake_message()) {
        *transaction.mutable_stake_message() = input.stake_message();
    }
    
    *transaction.mutable_signature() = sig;

    return transactionJSON(transaction);
}

std::string Signer::buildTransaction() const {    
    auto signature = sign();
    return buildTransactionJSON(signature).dump();
}

Proto::SigningOutput Signer::build() const {
    auto output = Proto::SigningOutput();

    auto signature = sign();
    auto txJson = buildTransactionJSON(signature);
    auto txEncoded = json::to_cbor(txJson);

    output.set_json(txJson.dump());
    output.set_encoded(txEncoded.data(), txEncoded.size());

    return output;
}