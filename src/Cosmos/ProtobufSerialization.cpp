// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "JsonSerialization.h"
#include "../proto/Cosmos.pb.h"
#include "Protobuf/coin.pb.h"
#include "Protobuf/bank_tx.pb.h"
#include "Protobuf/distribution_tx.pb.h"
#include "Protobuf/staking_tx.pb.h"
#include "Protobuf/tx.pb.h"
#include "Protobuf/crypto_secp256k1_keys.pb.h"

#include "PrivateKey.h"
#include "Data.h"
#include "Hash.h"

using namespace TW;
using namespace TW::Cosmos;

namespace TW::Cosmos {

const auto ProtobufAnyNamespacePrefix = "";  // to override default 'type.googleapis.com'

cosmos::base::v1beta1::Coin convertCoin(const Proto::Amount& amount) {
    cosmos::base::v1beta1::Coin coin;
    coin.set_denom(amount.denom());
    coin.set_amount(std::to_string(amount.amount()));
    return coin;
}

// Convert messages from external protobuf to internal protobuf
google::protobuf::Any convertMessage(const Proto::Message& msg) {
    google::protobuf::Any any;
    switch (msg.message_oneof_case()) {
        case Proto::Message::kSendCoinsMessage:
            {
                assert(msg.has_send_coins_message());
                const auto& send = msg.send_coins_message();
                auto msgSend = cosmos::bank::v1beta1::MsgSend();
                msgSend.set_from_address(send.from_address());
                msgSend.set_to_address(send.to_address());
                for (auto i = 0; i < send.amounts_size(); ++i) {
                    *msgSend.add_amount() = convertCoin(send.amounts(i));
                }
                any.PackFrom(msgSend, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kStakeMessage:
            {
                assert(msg.has_stake_message());
                const auto& stake = msg.stake_message();
                auto msgDelegate = cosmos::staking::v1beta1::MsgDelegate();
                msgDelegate.set_delegator_address(stake.delegator_address());
                msgDelegate.set_validator_address(stake.validator_address());
                *msgDelegate.mutable_amount() = convertCoin(stake.amount());
                any.PackFrom(msgDelegate, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kUnstakeMessage:
            {
                assert(msg.has_unstake_message());
                const auto& unstake = msg.unstake_message();
                auto msgUndelegate = cosmos::staking::v1beta1::MsgUndelegate();
                msgUndelegate.set_delegator_address(unstake.delegator_address());
                msgUndelegate.set_validator_address(unstake.validator_address());
                *msgUndelegate.mutable_amount() = convertCoin(unstake.amount());
                any.PackFrom(msgUndelegate, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kRestakeMessage:
            {
                assert(msg.has_restake_message());
                const auto& restake = msg.restake_message();
                auto msgRedelegate = cosmos::staking::v1beta1::MsgBeginRedelegate();
                msgRedelegate.set_delegator_address(restake.delegator_address());
                msgRedelegate.set_validator_src_address(restake.validator_src_address());
                msgRedelegate.set_validator_dst_address(restake.validator_dst_address());
                *msgRedelegate.mutable_amount() = convertCoin(restake.amount());
                any.PackFrom(msgRedelegate, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kWithdrawStakeRewardMessage:
            {
                assert(msg.has_withdraw_stake_reward_message());
                const auto& withdraw = msg.withdraw_stake_reward_message();
                auto msgWithdraw = cosmos::distribution::v1beta1::MsgWithdrawDelegatorReward();
                msgWithdraw.set_delegator_address(withdraw.delegator_address());
                msgWithdraw.set_validator_address(withdraw.validator_address());
                any.PackFrom(msgWithdraw, ProtobufAnyNamespacePrefix);
                return any;
            }

        default:
            throw std::invalid_argument(std::string("Message not supported ") + std::to_string(msg.message_oneof_case()));
    }
}

std::string buildProtoTxBody(const Proto::SigningInput& input) {
    if (input.messages_size() < 1) {
        throw std::invalid_argument("No message found");
    }
    assert(input.messages_size() >= 1);
    auto txBody = cosmos::TxBody();
    for (auto i = 0; i < input.messages_size(); ++i) {
        const auto msgAny = convertMessage(input.messages(i));
        *txBody.add_messages() = msgAny;
    }
    txBody.set_memo(input.memo());
    txBody.set_timeout_height(0);

    return txBody.SerializeAsString();
}

std::string buildAuthInfo(const Proto::SigningInput& input) {
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
    return authInfo.SerializeAsString();
}

Data buildSignature(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo) {
    // SignDoc Preimage
    auto signDoc = cosmos::SignDoc();
    signDoc.set_body_bytes(serializedTxBody);
    signDoc.set_auth_info_bytes(serializedAuthInfo);
    signDoc.set_chain_id(input.chain_id());
    signDoc.set_account_number(input.account_number());
    const auto serializedSignDoc = signDoc.SerializeAsString();

    auto hashToSign = Hash::sha256(serializedSignDoc);
    const auto privateKey = PrivateKey(input.private_key());
    auto signedHash = privateKey.sign(hashToSign, TWCurveSECP256k1);
    auto signature = Data(signedHash.begin(), signedHash.end() - 1);
    return signature;
}

std::string buildProtoTxRaw(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo, const Data& signature) {
    auto txRaw = cosmos::TxRaw();
    txRaw.set_body_bytes(serializedTxBody);
    txRaw.set_auth_info_bytes(serializedAuthInfo);
    *txRaw.add_signatures() = std::string(signature.begin(), signature.end());
    return txRaw.SerializeAsString();
}

} // namespace
