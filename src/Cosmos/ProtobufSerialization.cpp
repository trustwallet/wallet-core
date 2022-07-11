// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ProtobufSerialization.h"
#include "JsonSerialization.h"
#include "../proto/Cosmos.pb.h"
#include "Protobuf/coin.pb.h"
#include "Protobuf/bank_tx.pb.h"
#include "Protobuf/cosmwasm_wasm_v1_tx.pb.h"
#include "Protobuf/distribution_tx.pb.h"
#include "Protobuf/staking_tx.pb.h"
#include "Protobuf/tx.pb.h"
#include "Protobuf/crypto_secp256k1_keys.pb.h"
#include "Protobuf/ibc_applications_transfer_tx.pb.h"
#include "Protobuf/terra_wasm_v1beta1_tx.pb.h"
#include "Protobuf/thorchain_bank_tx.pb.h"
#include "Protobuf/ethermint_keys.pb.h"

#include "PrivateKey.h"
#include "Data.h"
#include "Hash.h"
#include "Base64.h"
#include "uint256.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;
using namespace TW::Cosmos;

namespace TW::Cosmos {

const auto ProtobufAnyNamespacePrefix = "";  // to override default 'type.googleapis.com'

cosmos::base::v1beta1::Coin convertCoin(const Proto::Amount& amount) {
    cosmos::base::v1beta1::Coin coin;
    coin.set_denom(amount.denom());
    coin.set_amount(amount.amount());
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

        case Proto::Message::kTransferTokensMessage:
            {
                assert(msg.has_transfer_tokens_message());
                const auto& transfer = msg.transfer_tokens_message();
                auto msgTransfer = ibc::applications::transfer::v1::MsgTransfer();
                msgTransfer.set_source_port(transfer.source_port());
                msgTransfer.set_source_channel(transfer.source_channel());
                *msgTransfer.mutable_token() = convertCoin(transfer.token());
                msgTransfer.set_sender(transfer.sender());
                msgTransfer.set_receiver(transfer.receiver());
                msgTransfer.mutable_timeout_height()->set_revision_number(transfer.timeout_height().revision_number());
                msgTransfer.mutable_timeout_height()->set_revision_height(transfer.timeout_height().revision_height());
                msgTransfer.set_timeout_timestamp(transfer.timeout_timestamp());
                any.PackFrom(msgTransfer, ProtobufAnyNamespacePrefix);
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

        case Proto::Message::kWasmTerraExecuteContractTransferMessage:
            {
                assert(msg.has_wasm_terra_execute_contract_transfer_message());
                const auto& wasmExecute = msg.wasm_terra_execute_contract_transfer_message();
                auto msgExecute = terra::wasm::v1beta1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                const std::string payload = Cosmos::wasmTerraExecuteTransferPayload(wasmExecute).dump();
                msgExecute.set_execute_msg(payload);
                any.PackFrom(msgExecute, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kWasmTerraExecuteContractSendMessage:
            {
                assert(msg.has_wasm_terra_execute_contract_send_message());
                const auto& wasmExecute = msg.wasm_terra_execute_contract_send_message();
                auto msgExecute = terra::wasm::v1beta1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                const std::string payload = Cosmos::wasmTerraExecuteSendPayload(wasmExecute).dump();
                msgExecute.set_execute_msg(payload);
                any.PackFrom(msgExecute, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kThorchainSendMessage:
            {
                assert(msg.has_thorchain_send_message());
                const auto& send = msg.thorchain_send_message();
                auto msgSend =types::MsgSend();
                msgSend.set_from_address(send.from_address());
                msgSend.set_to_address(send.to_address());
                for (auto i = 0; i < send.amounts_size(); ++i) {
                    *msgSend.add_amount() = convertCoin(send.amounts(i));
                }
                any.PackFrom(msgSend, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kWasmTerraExecuteContractGeneric: {
            assert(msg.has_wasm_terra_execute_contract_generic());
                const auto& wasmExecute = msg.wasm_terra_execute_contract_generic();
                auto msgExecute = terra::wasm::v1beta1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                msgExecute.set_execute_msg(wasmExecute.execute_msg());

                for (auto i = 0; i < wasmExecute.coins_size(); ++i) {
                    *msgExecute.add_coins() = convertCoin(wasmExecute.coins(i));
                }
                any.PackFrom(msgExecute, ProtobufAnyNamespacePrefix);
                return any;
        }

        case Proto::Message::kWasmExecuteContractTransferMessage:
            {
                assert(msg.has_wasm_execute_contract_transfer_message());
                const auto& wasmExecute = msg.wasm_execute_contract_transfer_message();
                auto msgExecute = cosmwasm::wasm::v1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                const std::string payload = Cosmos::wasmExecuteTransferPayload(wasmExecute).dump();
                msgExecute.set_msg(payload);
                any.PackFrom(msgExecute, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kWasmExecuteContractSendMessage:
            {
                assert(msg.has_wasm_execute_contract_send_message());
                const auto& wasmExecute = msg.wasm_execute_contract_send_message();
                auto msgExecute = cosmwasm::wasm::v1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                const std::string payload = Cosmos::wasmExecuteSendPayload(wasmExecute).dump();
                msgExecute.set_msg(payload);
                any.PackFrom(msgExecute, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kWasmExecuteContractGeneric: {
            assert(msg.has_wasm_execute_contract_generic());
                const auto& wasmExecute = msg.wasm_execute_contract_generic();
                auto msgExecute = cosmwasm::wasm::v1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                msgExecute.set_msg(wasmExecute.execute_msg());

                for (auto i = 0; i < wasmExecute.coins_size(); ++i) {
                    *msgExecute.add_funds() = convertCoin(wasmExecute.coins(i));
                }
                any.PackFrom(msgExecute, ProtobufAnyNamespacePrefix);
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

std::string buildAuthInfo(const Proto::SigningInput& input, TWCoinType coin) {
    // AuthInfo
    const auto privateKey = PrivateKey(input.private_key());
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    auto authInfo = cosmos::AuthInfo();
    auto* signerInfo = authInfo.add_signer_infos();

    signerInfo->mutable_mode_info()->mutable_single()->set_mode(cosmos::signing::v1beta1::SIGN_MODE_DIRECT);
    signerInfo->set_sequence(input.sequence());
    switch(coin) {
        case TWCoinTypeNativeEvmos: {
            auto pubKey = ethermint::crypto::v1::ethsecp256k1::PubKey();
            pubKey.set_key(publicKey.bytes.data(), publicKey.bytes.size());
            signerInfo->mutable_public_key()->PackFrom(pubKey, ProtobufAnyNamespacePrefix);
            break;
        }
        default: {
            auto pubKey = cosmos::crypto::secp256k1::PubKey();
            pubKey.set_key(publicKey.bytes.data(), publicKey.bytes.size());
            signerInfo->mutable_public_key()->PackFrom(pubKey, ProtobufAnyNamespacePrefix);
        }
    }

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

Data buildSignature(const Proto::SigningInput& input, const std::string& serializedTxBody, const std::string& serializedAuthInfo, TWCoinType coin) {
    // SignDoc Preimage
    auto signDoc = cosmos::SignDoc();
    signDoc.set_body_bytes(serializedTxBody);
    signDoc.set_auth_info_bytes(serializedAuthInfo);
    signDoc.set_chain_id(input.chain_id());
    signDoc.set_account_number(input.account_number());
    const auto serializedSignDoc = signDoc.SerializeAsString();

    Data hashToSign;
    switch(coin) {
        case TWCoinTypeNativeEvmos: {
            hashToSign = Hash::keccak256(serializedSignDoc);
            break;
        }
        default: {
            hashToSign = Hash::sha256(serializedSignDoc);
        }
    }

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

static string broadcastMode(Proto::BroadcastMode mode) {
    switch (mode) {
    case Proto::BroadcastMode::BLOCK:
        return "BROADCAST_MODE_BLOCK";
    case Proto::BroadcastMode::ASYNC:
        return "BROADCAST_MODE_ASYNC";
    case Proto::BroadcastMode::SYNC:
    default: return "BROADCAST_MODE_SYNC";
    }
}

std::string buildProtoTxJson(const Proto::SigningInput& input, const std::string& serializedTx) {
    const string serializedBase64 = Base64::encode(TW::data(serializedTx)); 
    const json jsonSerialized = {
        {"tx_bytes", serializedBase64},
        {"mode", broadcastMode(input.mode())}
    };
    return jsonSerialized.dump();
}

json wasmExecuteTransferPayload(const Proto::Message_WasmExecuteContractTransfer& msg) {
    return {
        {"transfer",
            {
                {"amount", toString(load(data(msg.amount())))},
                {"recipient", msg.recipient_address()}
            }
        }
    };
}

json wasmExecuteSendPayload(const Proto::Message_WasmExecuteContractSend& msg) {
    return {
        {"send",
            {
                {"amount", toString(load(data(msg.amount())))},
                {"contract", msg.recipient_contract_address()},
                {"msg", msg.msg()}
            }
        }
    };
}

json wasmTerraExecuteTransferPayload(const Proto::Message_WasmTerraExecuteContractTransfer& msg) {
    return {
        {"transfer",
            {
                {"amount", toString(load(data(msg.amount())))},
                {"recipient", msg.recipient_address()}
            }
        }
    };
}

json wasmTerraExecuteSendPayload(const Proto::Message_WasmTerraExecuteContractSend& msg) {
    return {
        {"send",
            {
                {"amount", toString(load(data(msg.amount())))},
                {"contract", msg.recipient_contract_address()},
                {"msg", msg.msg()}
            }
        }
    };
}

} // namespace
