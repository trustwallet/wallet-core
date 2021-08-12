// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "BaseTransaction.h"
#include "../HexCoding.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Algorand;

const Data TRANSACTION_TAG = {84, 88};
const std::string TRANSACTION_PAY = "pay";
const std::string ASSET_TRANSACTION = "axfer";

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto from = Address(pubkey);
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
        auto signature = sign(key, transaction);
        auto serialized = transaction.BaseTransaction::serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
    } else if (input.has_asset_transfer()) {
        auto message = input.asset_transfer();
        auto to = Address(message.to_address());

        auto transaction =
            AssetTransfer(from, to, fee, message.amount(),
                                            message.asset_id(), firstRound, lastRound, note,
                                    ASSET_TRANSACTION,genesisId, genesisHash);
        auto signature = sign(key, transaction);
        auto serialized = transaction.BaseTransaction::serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
    } else if (input.has_asset_opt_in()) {
        auto message = input.asset_opt_in();

        auto transaction = OptInAssetTransaction(from, fee, message.asset_id(),
                                                 firstRound, lastRound, note,
                                                 ASSET_TRANSACTION,genesisId, genesisHash);
        auto signature = sign(key, transaction);
        auto serialized = transaction.BaseTransaction::serialize(signature);
        protoOutput.set_encoded(serialized.data(), serialized.size());
    }
    
    return protoOutput;
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
    return Data(signature.begin(), signature.end());
}
