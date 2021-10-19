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
#include "Protobuf/crypto.secp256k1.pb.h"
#include "Protobuf/signing.pb.h"

#include "Data.h"
#include "Hash.h"
#include "Base64.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;

const auto ProtobufAnyNamespacePrefix = "";  // to override default 'type.googleapis.com'

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
cosmos::base::v1beta1::Coin convertCoin(const Proto::Amount amount) {
    cosmos::base::v1beta1::Coin coin;
    coin.set_denom(amount.denom());
    coin.set_amount(std::to_string(amount.amount()));
    return coin;
}

// TODO move to separate src file
cosmos::TxBody buildProtoTxBody(const Proto::SigningInput& input) noexcept {
    auto txBody = cosmos::TxBody();
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

    auto msgSend = cosmos::bank::v1beta1::MsgSend();
    msgSend.set_from_address(send.from_address());
    msgSend.set_to_address(send.to_address());
    for (auto i = 0; i < send.amounts_size(); ++i) {
        *msgSend.add_amount() = convertCoin(send.amounts(i));
    }

    txBody.add_messages()->PackFrom(msgSend, ProtobufAnyNamespacePrefix);
    txBody.set_memo(input.memo());
    txBody.set_timeout_height(0);

    return txBody;
}

// TODO move to separate src file
std::string buildProtoTxRaw(const Proto::SigningInput& input) noexcept {
    // TxBody
    const auto txBody = buildProtoTxBody(input);
    const auto serializedTxBody = txBody.SerializeAsString();

    // AuthInfo
    const auto privateKey = PrivateKey(input.private_key());
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto authInfo = cosmos::AuthInfo();
    auto* signerInfo = authInfo.add_signer_infos();
    auto pubKey = cosmos::crypto::secp256k1::PubKey();
    pubKey.set_key(publicKey.bytes.data(), publicKey.bytes.size());
    signerInfo->mutable_public_key()->PackFrom(pubKey, ProtobufAnyNamespacePrefix);
    signerInfo->mutable_mode_info()->mutable_single()->set_mode(cosmos::signing::v1beta1::SIGN_MODE_DIRECT);
    signerInfo->set_sequence(input.sequence());
    auto* fee = authInfo.mutable_fee();
    for (auto i = 0; i < input.fee().amounts_size(); ++i) {
        *fee->add_amount() = convertCoin(input.fee().amounts(i));
    }
    fee->set_gas_limit(input.fee().gas());
    fee->set_payer("");
    fee->set_granter("");
    // tip is omitted

    // SignDoc Preimage
    auto signDoc = cosmos::SignDoc();
    signDoc.set_body_bytes(serializedTxBody);
    signDoc.set_auth_info_bytes(authInfo.SerializeAsString());
    signDoc.set_chain_id(input.chain_id());
    signDoc.set_account_number(input.account_number());
    const auto serializedSignDoc = signDoc.SerializeAsString();

    // Signature
    auto hash = Hash::sha256(serializedSignDoc);
    auto signedHash = privateKey.sign(hash, TWCurveSECP256k1);
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);

    auto txRaw = cosmos::TxRaw();
    txRaw.set_body_bytes(serializedTxBody);
    txRaw.set_auth_info_bytes(authInfo.SerializeAsString());
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
