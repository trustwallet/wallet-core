// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ProtobufSerialization.h"

#include "Base64.h"
#include "Constants.h"
#include "Cosmos/Protobuf/bank_tx.pb.h"
#include "Cosmos/Protobuf/greenfield_ethsecp256k1.pb.h"
#include "Cosmos/Protobuf/greenfield_tx.pb.h"
#include "Cosmos/Protobuf/tx.pb.h"
#include "PrivateKey.h"

namespace TW::Greenfield {

static constexpr auto ProtobufAnyNamespacePrefix = "";  // to override default 'type.googleapis.com'

using Any = google::protobuf::Any;

static std::string broadcastMode(Proto::BroadcastMode mode) {
    switch (mode) {
        case Proto::BroadcastMode::ASYNC:
            return "BROADCAST_MODE_ASYNC";
        case Proto::BroadcastMode::SYNC:
        default: return "BROADCAST_MODE_SYNC";
    }
}

static json broadcastJSON(std::string data, Proto::BroadcastMode mode) {
    return {
        {"tx_bytes", data},
        {"mode", broadcastMode(mode)}
    };
}

static cosmos::base::v1beta1::Coin convertCoin(const Proto::Amount& amount) {
    cosmos::base::v1beta1::Coin coin;
    coin.set_denom(amount.denom());
    coin.set_amount(amount.amount());
    return coin;
}

static SigningResult<Any> convertMessage(const Proto::Message& msg) {
    Any any;

    switch (msg.message_oneof_case()) {
        case Proto::Message::kSendCoinsMessage: {
            const auto& send = msg.send_coins_message();

            auto msgSend = cosmos::bank::v1beta1::MsgSend();
            msgSend.set_from_address(send.from_address());
            msgSend.set_to_address(send.to_address());

            for (auto i = 0; i < send.amounts_size(); ++i) {
                *msgSend.add_amount() = convertCoin(send.amounts(i));
            }
            any.PackFrom(msgSend, ProtobufAnyNamespacePrefix);
            break;
        }
        case Proto::Message::kBridgeTransferOut: {
            const auto& transferOut = msg.bridge_transfer_out();

            auto msgTransferOut = greenfield::bridge::MsgTransferOut();
            msgTransferOut.set_from(transferOut.from_address());
            msgTransferOut.set_to(transferOut.to_address());
            *msgTransferOut.mutable_amount() = convertCoin(transferOut.amount());

            any.PackFrom(msgTransferOut, ProtobufAnyNamespacePrefix);
            break;
        }
        default: {
            return SigningResult<Any>::failure(Common::Proto::SigningError::Error_invalid_params);
        }
    }

    return SigningResult<Any>::success(std::move(any));
}

SigningResult<json> ProtobufSerialization::encodeTxProtobuf(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature) {
    const auto txBodyResult = encodeTxBody(input);
    const auto serializedAuthInfo = encodeAuthInfo(input, publicKey);

    if (txBodyResult.isFailure()) {
        return SigningResult<json>::failure(txBodyResult.error());
    }
    const auto serializedTxBody = txBodyResult.payload();

    auto txRaw = cosmos::TxRaw();
    txRaw.set_body_bytes(serializedTxBody.data(), serializedTxBody.size());
    txRaw.set_auth_info_bytes(serializedAuthInfo.data(), serializedAuthInfo.size());
    *txRaw.add_signatures() = std::string(signature.begin(), signature.end());

    const auto txRawData = data(txRaw.SerializeAsString());
    auto txJson = broadcastJSON(Base64::encode(txRawData), input.mode());

    return SigningResult<json>::success(std::move(txJson));
}

SigningResult<Data> ProtobufSerialization::encodeTxBody(const Proto::SigningInput& input) {
    cosmos::TxBody txBody;

    // At this moment, we support only one message.
    if (input.messages_size() != 1) {
        return SigningResult<Data>::failure(Common::Proto::SigningError::Error_invalid_params);
    }
    const auto msgAny = convertMessage(input.messages(0));
    if (msgAny.isFailure()) {
        return SigningResult<Data>::failure(msgAny.error());
    }

    *txBody.add_messages() = msgAny.payload();
    txBody.set_memo(input.memo());
    txBody.set_timeout_height(TIMEOUT_HEIGHT);

    return SigningResult<Data>::success(data(txBody.SerializeAsString()));
}

Data ProtobufSerialization::encodeAuthInfo(const Proto::SigningInput& input, const PublicKey& publicKey) {
    // AuthInfo
    auto authInfo = cosmos::AuthInfo();
    auto* signerInfo = authInfo.add_signer_infos();

    // At this moment, we support Eip712 signing mode only.
    signerInfo->mutable_mode_info()->mutable_single()->set_mode(cosmos::signing::v1beta1::SIGN_MODE_EIP_712);
    signerInfo->set_sequence(input.sequence());

    // `cosmos::crypto::eth::ethsecp256k1` is used only with `SIGN_MODE_EIP_712`.
    auto pubKey = cosmos::crypto::eth::ethsecp256k1::PubKey();
    pubKey.set_key(publicKey.bytes.data(), publicKey.bytes.size());
    signerInfo->mutable_public_key()->PackFrom(pubKey, ProtobufAnyNamespacePrefix);

    auto& fee = *authInfo.mutable_fee();
    for (auto i = 0; i < input.fee().amounts_size(); ++i) {
        *fee.add_amount() = convertCoin(input.fee().amounts(i));
    }

    fee.set_gas_limit(input.fee().gas());
    fee.set_payer("");
    fee.set_granter("");
    // tip is omitted
    return data(authInfo.SerializeAsString());
}

} // namespace TW::Greenfield
