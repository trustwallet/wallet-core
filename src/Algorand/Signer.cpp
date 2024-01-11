// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Address.h"
#include "BaseTransaction.h"
#include "Base64.h"
#include "../HexCoding.h"

#include <google/protobuf/util/json_util.h>

namespace TW::Algorand {

const Data TRANSACTION_TAG = {84, 88};
const std::string TRANSACTION_PAY = "pay";
const std::string ASSET_TRANSACTION = "axfer";

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);

    auto preImageData = Signer::preImage(pubkey, input);
    auto signature = key.sign(preImageData, TWCurveED25519);
    return Signer::encodeTransaction(signature, pubkey, input);
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    return hex(Signer::sign(input).encoded());
}

Data Signer::sign(const PrivateKey& privateKey, const BaseTransaction& transaction) noexcept {
    Data data;
    append(data, TRANSACTION_TAG);
    append(data, transaction.serialize());
    auto signature = privateKey.sign(data, TWCurveED25519);
    return {signature.begin(), signature.end()};
}

TW::Data Signer::signaturePreimage(const Proto::SigningInput& input) noexcept {
    auto pubKey = input.public_key();
    return Signer::preImage(PublicKey(Data(pubKey.begin(), pubKey.end()), TWPublicKeyTypeED25519), input);
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey, const Proto::SigningInput& input) noexcept {
    return Signer::encodeTransaction(signature, publicKey, input);
}

TW::Data Signer::preImage(const TW::PublicKey& pubKey, const Proto::SigningInput& input) noexcept {
    auto from = Address(pubKey);
    auto firstRound = input.first_round();
    auto lastRound = input.last_round();
    auto fee = input.fee();

    auto note = Data(input.note().begin(), input.note().end());
    auto genesisId = input.genesis_id();
    auto genesisHash = Data(input.genesis_hash().begin(), input.genesis_hash().end());

    TW::Data transactionData;
    if (input.has_transfer()) {
        auto message = input.transfer();
        auto to = Address(message.to_address());

        auto transaction = Transfer(from, to, fee, message.amount(), firstRound,
                                    lastRound, note, TRANSACTION_PAY, genesisId, genesisHash);
        transactionData = transaction.serialize();
    } else if (input.has_asset_transfer()) {
        auto message = input.asset_transfer();
        auto to = Address(message.to_address());

        auto transaction =
            AssetTransfer(from, to, fee, message.amount(),
                          message.asset_id(), firstRound, lastRound, note,
                          ASSET_TRANSACTION, genesisId, genesisHash);
        transactionData = transaction.serialize();
    } else if (input.has_asset_opt_in()) {
        auto message = input.asset_opt_in();
        auto transaction = OptInAssetTransaction(from, fee, message.asset_id(),
                                                 firstRound, lastRound, note,
                                                 ASSET_TRANSACTION, genesisId, genesisHash);
        transactionData = transaction.serialize();
    } else {
        return {Data{}};
    }

    Data data;
    append(data, TRANSACTION_TAG);
    append(data, transactionData);
    return data;
}

Proto::SigningOutput Signer::encodeTransaction(const Data& signature, const TW::PublicKey& pubKey, const Proto::SigningInput& input) noexcept {
    auto protoOutput = Proto::SigningOutput();

    auto from = Address(pubKey);
    auto firstRound = input.first_round();
    auto lastRound = input.last_round();
    auto fee = input.fee();

    auto note = Data(input.note().begin(), input.note().end());
    auto genesisId = input.genesis_id();
    auto genesisHash = Data(input.genesis_hash().begin(), input.genesis_hash().end());
    if (input.has_transfer()) {
        auto message = input.transfer();
        auto to = Address(message.to_address());

        auto transaction = Transfer(from, to, fee, message.amount(), firstRound,
                                    lastRound, note, TRANSACTION_PAY, genesisId, genesisHash);

        auto serialized = transaction.BaseTransaction::serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
        protoOutput.set_signature(Base64::encode(signature));
    } else if (input.has_asset_transfer()) {
        auto message = input.asset_transfer();
        auto to = Address(message.to_address());

        auto transaction =
            AssetTransfer(from, to, fee, message.amount(),
                          message.asset_id(), firstRound, lastRound, note,
                          ASSET_TRANSACTION, genesisId, genesisHash);

        auto serialized = transaction.BaseTransaction::serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
        protoOutput.set_signature(Base64::encode(signature));
    } else if (input.has_asset_opt_in()) {
        auto message = input.asset_opt_in();

        auto transaction = OptInAssetTransaction(from, fee, message.asset_id(),
                                                 firstRound, lastRound, note,
                                                 ASSET_TRANSACTION, genesisId, genesisHash);

        auto serialized = transaction.BaseTransaction::serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
        protoOutput.set_signature(Base64::encode(signature));
    }
    return protoOutput;
}

} // namespace TW::Algorand
