// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../proto/Cosmos.pb.h"
#include "PrivateKey.h"
#include "JsonSerialization.h"
#include "Protobuf/bank.tx.pb.h"
#include "Protobuf/tx.pb.h"

#include "Data.h"
#include "Hash.h"
#include "Base64.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    switch (input.signing_mode()) {
        case Proto::JSON:
            return signJsonSerialized(input);
        
        case Proto::Protobuf:
        default:
            return signProtobuf(input);
    }
}

Proto::SigningOutput Signer::signJsonSerialized(const Proto::SigningInput& input) noexcept {
    auto key = PrivateKey(input.private_key());
    auto preimage = signaturePreimageJSON(input).dump();
    auto hash = Hash::sha256(preimage);
    auto signedHash = key.sign(hash, TWCurveSECP256k1);

    auto output = Proto::SigningOutput();
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);
    auto txJson = transactionJSON(input, signature);
    output.set_json(txJson.dump());
    output.set_signature(signature.data(), signature.size());
    output.set_serialized("");
    output.set_serialized_base64("");
    return output;
}

// TODO move to separate src file
cosmos::proto::base::v1beta1::Coin convertCoin(const Proto::Amount amount) {
    cosmos::proto::base::v1beta1::Coin coin;
    coin.set_denom(amount.denom());
    coin.set_amount(std::to_string(amount.amount()));
    return coin;
}

// TODO move to separate src file
cosmos::proto::TxBody buildProtoTxBody(const Proto::SigningInput& input) noexcept {
    auto txBody = cosmos::proto::TxBody();

    if (input.messages_size() < 1) {
        // TODO support multiple msgs
        return txBody;
    }
    assert(input.messages_size() >= 1);
    if (!input.messages(0).has_send_coins_message()) {
        // TODO support other msgs
        return txBody;
    }
    assert(input.messages(0).has_send_coins_message());
    const Proto::Message::Send& send = input.messages(0).send_coins_message();

    auto msgSend = cosmos::proto::bank::v1beta1::MsgSend();
    msgSend.set_from_address(send.from_address());
    msgSend.set_to_address(send.to_address());
    for (auto i = 0; i < send.amounts_size(); ++i) {
        *msgSend.add_amount() = convertCoin(send.amounts(i));
    }

    txBody.add_messages()->PackFrom(msgSend);
    txBody.set_memo(input.memo());
    txBody.set_timeout_height(0);

    return txBody;
}

// TODO move to separate src file
std::string buildProtoTxRaw(const Proto::SigningInput& input) noexcept {
    auto txRaw = cosmos::proto::TxRaw();

    // Preimage
    const auto txBody = buildProtoTxBody(input);
    const auto serializedTxBody = txBody.SerializeAsString();

    // Signature
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(serializedTxBody);
    auto signedHash = key.sign(hash, TWCurveSECP256k1);
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);

    txRaw.set_body_bytes(serializedTxBody);
    txRaw.set_auth_info_bytes(""); // TODO
    *txRaw.add_signatures() = std::string(signature.begin(), signature.end());

    return txRaw.SerializeAsString();
}

Proto::SigningOutput Signer::signProtobuf(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();

    // Preimage
    auto serializedTxRaw = buildProtoTxRaw(input);

    // TODO prevent signing again, reuse from build...
    const auto serializedTxBody = buildProtoTxBody(input).SerializeAsString();
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha256(serializedTxBody);
    auto signedHash = key.sign(hash, TWCurveSECP256k1);
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);

    // TODO: TxBody, TxRaw
    output.set_serialized(serializedTxRaw);
    output.set_serialized_base64(Base64::encode(TW::data(serializedTxRaw)));
    output.set_json("");
    output.set_signature(signature.data(), signature.size());
    return output;
}

std::string Signer::signJSON(const std::string& json, const Data& key) {
    auto input = Proto::SigningInput();
    google::protobuf::util::JsonStringToMessage(json, &input);
    input.set_private_key(key.data(), key.size());
    auto output = Signer::sign(input);
    return output.json();
}
