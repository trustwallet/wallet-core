// Copyright © 2017-2023 Trust Wallet.
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
#include "Protobuf/authz_tx.pb.h"
#include "Protobuf/tx.pb.h"
#include "Protobuf/stride_liquid_staking.pb.h"
#include "Protobuf/gov_tx.pb.h"
#include "Protobuf/crypto_secp256k1_keys.pb.h"
#include "Protobuf/terra_wasm_v1beta1_tx.pb.h"
#include "Protobuf/ibc_applications_transfer_tx.pb.h"
#include "Protobuf/thorchain_bank_tx.pb.h"
#include "Protobuf/ethermint_keys.pb.h"
#include "Protobuf/injective_keys.pb.h"

#include "PrivateKey.h"
#include "Data.h"
#include "Hash.h"
#include "Base64.h"
#include "uint256.h"

#include <google/protobuf/util/json_util.h>

using namespace TW;

namespace TW::Cosmos::Protobuf {

using json = nlohmann::json;
using string = std::string;
const auto ProtobufAnyNamespacePrefix = "";  // to override default 'type.googleapis.com'

static string broadcastMode(Proto::BroadcastMode mode) {
    switch (mode) {
    case Proto::BroadcastMode::BLOCK:
        return "BROADCAST_MODE_BLOCK";
    case Proto::BroadcastMode::ASYNC:
        return "BROADCAST_MODE_ASYNC";
    default: return "BROADCAST_MODE_SYNC";
    }
}

static json broadcastJSON(std::string data, Proto::BroadcastMode mode) {
    return {
        {"tx_bytes", data},
        {"mode", broadcastMode(mode)}
    };
}

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

        case Proto::Message::kSetWithdrawAddressMessage:
            {
                assert(msg.has_set_withdraw_address_message());
                const auto& withdraw = msg.set_withdraw_address_message();
                auto msgWithdraw = cosmos::distribution::v1beta1::MsgSetWithdrawAddress();
                msgWithdraw.set_delegator_address(withdraw.delegator_address());
                msgWithdraw.set_withdraw_address(withdraw.withdraw_address());
                any.PackFrom(msgWithdraw, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kExecuteContractMessage:
            {
                assert(msg.has_execute_contract_message());
                const auto& execContract = msg.execute_contract_message();
                auto executeContractMsg = terra::wasm::v1beta1::MsgExecuteContract();
                executeContractMsg.set_sender(execContract.sender());
                executeContractMsg.set_contract(execContract.contract());
                executeContractMsg.set_execute_msg(execContract.execute_msg());
                for (auto i = 0; i < execContract.coins_size(); ++i){
                    *executeContractMsg.add_coins() = convertCoin(execContract.coins(i));
                }
                any.PackFrom(executeContractMsg, ProtobufAnyNamespacePrefix);
                return any;
            }

        case Proto::Message::kWasmTerraExecuteContractTransferMessage:
            {
                assert(msg.has_wasm_terra_execute_contract_transfer_message());
                const auto& wasmExecute = msg.wasm_terra_execute_contract_transfer_message();
                auto msgExecute = terra::wasm::v1beta1::MsgExecuteContract();
                msgExecute.set_sender(wasmExecute.sender_address());
                msgExecute.set_contract(wasmExecute.contract_address());
                const std::string payload = wasmTerraExecuteTransferPayload(wasmExecute).dump();
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
                const std::string payload = wasmTerraExecuteSendPayload(wasmExecute).dump();
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
                const std::string payload = wasmExecuteTransferPayload(wasmExecute).dump();
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
                const std::string payload = wasmExecuteSendPayload(wasmExecute).dump();
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

        case Proto::Message::kAuthGrant: {
                assert(msg.has_auth_grant());
                const auto& authGrant = msg.auth_grant();
                auto msgAuthGrant = cosmos::authz::v1beta1::MsgGrant();
                msgAuthGrant.set_grantee(authGrant.grantee());
                msgAuthGrant.set_granter(authGrant.granter());
                auto* mtAuth = msgAuthGrant.mutable_grant()->mutable_authorization();
                // There is multiple grant possibilities, but we add support staking/compounding only for now.
                switch (authGrant.grant_type_case()) {
                case Proto::Message_AuthGrant::kGrantStake:
                    mtAuth->PackFrom(authGrant.grant_stake(), ProtobufAnyNamespacePrefix);
                    mtAuth->set_type_url("/cosmos.staking.v1beta1.StakeAuthorization");
                    break;
                case Proto::Message_AuthGrant::GRANT_TYPE_NOT_SET:
                    break;
                }
                auto* mtExp = msgAuthGrant.mutable_grant()->mutable_expiration();
                mtExp->set_seconds(authGrant.expiration());
                any.PackFrom(msgAuthGrant, ProtobufAnyNamespacePrefix);
                return any;
        }

        case Proto::Message::kAuthRevoke: {
            assert(msg.has_auth_revoke());
            const auto& authRevoke = msg.auth_revoke();
            auto msgAuthRevoke = cosmos::authz::v1beta1::MsgRevoke();
            msgAuthRevoke.set_granter(authRevoke.granter());
            msgAuthRevoke.set_grantee(authRevoke.grantee());
            msgAuthRevoke.set_msg_type_url(authRevoke.msg_type_url());
            any.PackFrom(msgAuthRevoke, ProtobufAnyNamespacePrefix);
            return any;
        }
        case Proto::Message::kMsgVote: {
            assert(msg.has_msg_vote());
            const auto& vote = msg.msg_vote();
            auto msgVote = cosmos::gov::v1beta1::MsgVote();
            // LCOV_EXCL_START
            switch (vote.option()) {
            case Proto::Message_VoteOption__UNSPECIFIED:
                msgVote.set_option(cosmos::gov::v1beta1::VOTE_OPTION_UNSPECIFIED);
                break;
            case Proto::Message_VoteOption_YES:
                msgVote.set_option(cosmos::gov::v1beta1::VOTE_OPTION_YES);
                break;
            case Proto::Message_VoteOption_ABSTAIN:
                msgVote.set_option(cosmos::gov::v1beta1::VOTE_OPTION_ABSTAIN);
                break;
            case Proto::Message_VoteOption_NO:
                msgVote.set_option(cosmos::gov::v1beta1::VOTE_OPTION_NO);
                break;
            case Proto::Message_VoteOption_NO_WITH_VETO:
                msgVote.set_option(cosmos::gov::v1beta1::VOTE_OPTION_NO_WITH_VETO);
                break;
            case Proto::Message_VoteOption_Message_VoteOption_INT_MIN_SENTINEL_DO_NOT_USE_:
                msgVote.set_option(cosmos::gov::v1beta1::VoteOption_INT_MIN_SENTINEL_DO_NOT_USE_);
                break;
            case Proto::Message_VoteOption_Message_VoteOption_INT_MAX_SENTINEL_DO_NOT_USE_:
                msgVote.set_option(cosmos::gov::v1beta1::VoteOption_INT_MAX_SENTINEL_DO_NOT_USE_);
                break;
            }
            // LCOV_EXCL_STOP
            msgVote.set_proposal_id(vote.proposal_id());
            msgVote.set_voter(vote.voter());
            any.PackFrom(msgVote, ProtobufAnyNamespacePrefix);
            return any;
        }
        case Proto::Message::kMsgStrideLiquidStakingStake: {
                const auto& stride_liquid_staking_stake = msg.msg_stride_liquid_staking_stake();
                auto liquid_staking_msg = stride::stakeibc::MsgLiquidStake();
                liquid_staking_msg.set_creator(stride_liquid_staking_stake.creator());
                liquid_staking_msg.set_amount(stride_liquid_staking_stake.amount());
                liquid_staking_msg.set_host_denom(stride_liquid_staking_stake.host_denom());
                any.PackFrom(liquid_staking_msg, ProtobufAnyNamespacePrefix);
                return any;
        }
        case Proto::Message::kMsgStrideLiquidStakingRedeem: {
                const auto& stride_liquid_staking_redeem = msg.msg_stride_liquid_staking_redeem();
                auto liquid_staking_msg = stride::stakeibc::MsgRedeemStake();
                liquid_staking_msg.set_creator(stride_liquid_staking_redeem.creator());
                liquid_staking_msg.set_amount(stride_liquid_staking_redeem.amount());
                liquid_staking_msg.set_receiver(stride_liquid_staking_redeem.receiver());
                liquid_staking_msg.set_host_zone(stride_liquid_staking_redeem.host_zone());
                any.PackFrom(liquid_staking_msg, ProtobufAnyNamespacePrefix);
                return any;
        }
        case Proto::Message::kThorchainDepositMessage: {
                assert(msg.has_thorchain_deposit_message());
                const auto& deposit = msg.thorchain_deposit_message();
                types::MsgDeposit msgDeposit;
                msgDeposit.set_memo(deposit.memo());
                msgDeposit.set_signer(deposit.signer());
                for (auto i = 0; i < deposit.coins_size(); ++i) {
                    auto* coin = msgDeposit.add_coins();
                    auto originalAsset = deposit.coins(i).asset();
                    coin->mutable_asset()->set_chain(originalAsset.chain());
                    coin->mutable_asset()->set_symbol(originalAsset.symbol());
                    coin->mutable_asset()->set_ticker(originalAsset.ticker());
                    coin->mutable_asset()->set_synth(originalAsset.synth());
                    coin->set_amount(deposit.coins(i).amount());
                    coin->set_decimals(deposit.coins(i).decimals());
                }
                any.PackFrom(msgDeposit, ProtobufAnyNamespacePrefix);

                return any;
        }

        default:
            throw std::invalid_argument(std::string("Message not supported ") + std::to_string(msg.message_oneof_case()));
    }
}

std::string buildProtoTxBody(const Proto::SigningInput& input) {
    if (input.messages_size() >= 1 && input.messages(0).has_sign_direct_message()) {
        return input.messages(0).sign_direct_message().body_bytes();
    }

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
    return buildAuthInfo(input, publicKey, coin);
}

std::string buildAuthInfo(const Proto::SigningInput& input, const PublicKey& publicKey, TWCoinType coin) {
    if (input.messages_size() >= 1 && input.messages(0).has_sign_direct_message()) {
        return input.messages(0).sign_direct_message().auth_info_bytes();
    }
    // AuthInfo
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
        case TWCoinTypeNativeInjective: {
            auto pubKey = injective::crypto::v1beta1::ethsecp256k1::PubKey();
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
        case TWCoinTypeNativeInjective:
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

std::string buildProtoTxRaw(const std::string& serializedTxBody, const std::string& serializedAuthInfo, const Data& signature) {
    auto txRaw = cosmos::TxRaw();
    txRaw.set_body_bytes(serializedTxBody);
    txRaw.set_auth_info_bytes(serializedAuthInfo);
    *txRaw.add_signatures() = std::string(signature.begin(), signature.end());
    return txRaw.SerializeAsString();
}

std::string signaturePreimageProto(const Proto::SigningInput& input, const PublicKey& publicKey, TWCoinType coin) {
    // SignDoc Preimage
    const auto serializedTxBody = buildProtoTxBody(input);
    const auto serializedAuthInfo = buildAuthInfo(input, publicKey, coin);

    auto signDoc = cosmos::SignDoc();
    signDoc.set_body_bytes(serializedTxBody);
    signDoc.set_auth_info_bytes(serializedAuthInfo);
    signDoc.set_chain_id(input.chain_id());
    signDoc.set_account_number(input.account_number());
    return signDoc.SerializeAsString();
}

std::string buildProtoTxRaw(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature, TWCoinType coin) {
    const auto serializedTxBody = buildProtoTxBody(input);
    const auto serializedAuthInfo = buildAuthInfo(input, publicKey, coin);

    auto txRaw = cosmos::TxRaw();
    txRaw.set_body_bytes(serializedTxBody);
    txRaw.set_auth_info_bytes(serializedAuthInfo);
    *txRaw.add_signatures() = std::string(signature.begin(), signature.end());
    auto data = txRaw.SerializeAsString();
    return broadcastJSON(Base64::encode(Data(data.begin(), data.end())), input.mode()).dump();
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
