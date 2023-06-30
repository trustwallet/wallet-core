// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "Protobuf/TronInternal.pb.h"

#include "Serialization.h"
#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <cassert>
#include <chrono>

namespace TW::Tron {

const std::string TRANSFER_TOKEN_FUNCTION = "0xa9059cbb";

/// Converts an external TransferContract to an internal one used for signing.
protocol::TransferContract to_internal(const Proto::TransferContract& transfer) {
    auto internal = protocol::TransferContract();

    const auto ownerAddress = Base58::decodeCheck(transfer.owner_address());
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    const auto toAddress = Base58::decodeCheck(transfer.to_address());
    internal.set_to_address(toAddress.data(), toAddress.size());

    internal.set_amount(transfer.amount());

    return internal;
}

/// Converts an external TransferAssetContract to an internal one used for
/// signing.
protocol::TransferAssetContract to_internal(const Proto::TransferAssetContract& transfer) {
    auto internal = protocol::TransferAssetContract();

    internal.set_asset_name(transfer.asset_name());

    const auto ownerAddress = Base58::decodeCheck(transfer.owner_address());
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    const auto toAddress = Base58::decodeCheck(transfer.to_address());
    internal.set_to_address(toAddress.data(), toAddress.size());

    internal.set_amount(transfer.amount());

    return internal;
}

protocol::FreezeBalanceContract to_internal(const Proto::FreezeBalanceContract& freezeContract) {
    auto internal = protocol::FreezeBalanceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = Base58::decodeCheck(freezeContract.owner_address());
    const auto receiverAddress = Base58::decodeCheck(freezeContract.receiver_address());

    protocol::ResourceCode_Parse(freezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_receiver_address(receiverAddress.data(), receiverAddress.size());
    internal.set_frozen_balance(freezeContract.frozen_balance());
    internal.set_frozen_duration(freezeContract.frozen_duration());

    return internal;
}

protocol::FreezeBalanceV2Contract to_internal(const Proto::FreezeBalanceV2Contract& freezeContract) {
    auto internal = protocol::FreezeBalanceV2Contract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = Base58::decodeCheck(freezeContract.owner_address());

    protocol::ResourceCode_Parse(freezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_frozen_balance(freezeContract.frozen_balance());

    return internal;
}

protocol::UnfreezeBalanceContract to_internal(const Proto::UnfreezeBalanceContract& unfreezeContract) {
    auto internal = protocol::UnfreezeBalanceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = Base58::decodeCheck(unfreezeContract.owner_address());
    const auto receiverAddress = Base58::decodeCheck(unfreezeContract.receiver_address());

    protocol::ResourceCode_Parse(unfreezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_receiver_address(receiverAddress.data(), receiverAddress.size());

    return internal;
}

protocol::UnfreezeBalanceV2Contract to_internal(const Proto::UnfreezeBalanceV2Contract& unfreezeContract) {
    auto internal = protocol::UnfreezeBalanceV2Contract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = Base58::decodeCheck(unfreezeContract.owner_address());

    protocol::ResourceCode_Parse(unfreezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_unfreeze_balance(unfreezeContract.unfreeze_balance());

    return internal;
}

protocol::DelegateResourceContract to_internal(const Proto::DelegateResourceContract& delegateContract) {
    auto internal = protocol::DelegateResourceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = Base58::decodeCheck(delegateContract.owner_address());
    const auto receiverAddress = Base58::decodeCheck(delegateContract.receiver_address());

    protocol::ResourceCode_Parse(delegateContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_receiver_address(receiverAddress.data(), receiverAddress.size());
    internal.set_balance(delegateContract.balance());
    internal.set_lock(delegateContract.lock());

    return internal;
}

protocol::UnDelegateResourceContract to_internal(const Proto::UnDelegateResourceContract& undelegateContract) {
    auto internal = protocol::UnDelegateResourceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = Base58::decodeCheck(undelegateContract.owner_address());
    const auto receiverAddress = Base58::decodeCheck(undelegateContract.receiver_address());

    protocol::ResourceCode_Parse(undelegateContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_receiver_address(receiverAddress.data(), receiverAddress.size());
    internal.set_balance(undelegateContract.balance());

    return internal;
}

protocol::WithdrawExpireUnfreezeContract to_internal(const Proto::WithdrawExpireUnfreezeContract& withdrawExpireUnfreezeContract) {
    auto internal = protocol::WithdrawExpireUnfreezeContract();
    const auto ownerAddress = Base58::decodeCheck(withdrawExpireUnfreezeContract.owner_address());
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    return internal;
}

protocol::UnfreezeAssetContract to_internal(const Proto::UnfreezeAssetContract& unfreezeContract) {
    auto internal = protocol::UnfreezeAssetContract();
    const auto ownerAddress = Base58::decodeCheck(unfreezeContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    return internal;
}

protocol::VoteAssetContract to_internal(const Proto::VoteAssetContract& voteContract) {
    auto internal = protocol::VoteAssetContract();
    const auto ownerAddress = Base58::decodeCheck(voteContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_support(voteContract.support());
    internal.set_count(voteContract.count());
    for (int i = 0; i < voteContract.vote_address_size(); i++) {
        auto voteAddress = Base58::decodeCheck(voteContract.vote_address(i));
        internal.add_vote_address(voteAddress.data(), voteAddress.size());
    }

    return internal;
}

protocol::VoteWitnessContract to_internal(const Proto::VoteWitnessContract& voteContract) {
    auto internal = protocol::VoteWitnessContract();
    const auto ownerAddress = Base58::decodeCheck(voteContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_support(voteContract.support());
    for (int i = 0; i < voteContract.votes_size(); i++) {
        auto voteAddress = Base58::decodeCheck(voteContract.votes(i).vote_address());
        auto* vote = internal.add_votes();

        vote->set_vote_address(voteAddress.data(), voteAddress.size());
        vote->set_vote_count(voteContract.votes(i).vote_count());
    }

    return internal;
}

protocol::WithdrawBalanceContract to_internal(const Proto::WithdrawBalanceContract& withdrawContract) {
    auto internal = protocol::WithdrawBalanceContract();
    const auto ownerAddress = Base58::decodeCheck(withdrawContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    return internal;
}

protocol::TriggerSmartContract to_internal(const Proto::TriggerSmartContract& triggerSmartContract) {
    auto internal = protocol::TriggerSmartContract();
    const auto ownerAddress = Base58::decodeCheck(triggerSmartContract.owner_address());
    const auto contractAddress = Base58::decodeCheck(triggerSmartContract.contract_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_contract_address(contractAddress.data(), contractAddress.size());
    internal.set_call_value(triggerSmartContract.call_value());
    internal.set_data(triggerSmartContract.data().data(), triggerSmartContract.data().size());
    internal.set_call_token_value(triggerSmartContract.call_token_value());
    internal.set_token_id(triggerSmartContract.token_id());

    return internal;
}

protocol::TriggerSmartContract to_internal(const Proto::TransferTRC20Contract& transferTrc20Contract) {
    auto toAddress = Base58::decodeCheck(transferTrc20Contract.to_address());
    // amount is 256 bits, big endian
    Data amount = data(transferTrc20Contract.amount());

    // Encode smart contract call parameters
    auto contract_params = parse_hex(TRANSFER_TOKEN_FUNCTION);
    pad_left(toAddress, 32);
    pad_left(amount, 32);
    append(contract_params, toAddress);
    append(contract_params, amount);

    auto triggerSmartContract = Proto::TriggerSmartContract();
    triggerSmartContract.set_owner_address(transferTrc20Contract.owner_address());
    triggerSmartContract.set_contract_address(transferTrc20Contract.contract_address());
    triggerSmartContract.set_data(contract_params.data(), contract_params.size());

    return to_internal(triggerSmartContract);
}

/// Converts an external BlockHeader to an internal one used for signing.
protocol::BlockHeader to_internal(const Proto::BlockHeader& header) {
    auto internal = protocol::BlockHeader();

    internal.mutable_raw_data()->set_timestamp(header.timestamp());
    internal.mutable_raw_data()->set_tx_trie_root(header.tx_trie_root());
    internal.mutable_raw_data()->set_parent_hash(header.parent_hash());
    internal.mutable_raw_data()->set_number(header.number());
    internal.mutable_raw_data()->set_witness_address(header.witness_address());
    internal.mutable_raw_data()->set_version(header.version());

    return internal;
}

Data getBlockHash(const protocol::BlockHeader& header) {
    const auto data = header.raw_data().SerializeAsString();
    return Hash::sha256(data);
}

void setBlockReference(const Proto::Transaction& transaction, protocol::Transaction& internal) {
    const auto blockHash = getBlockHash(to_internal(transaction.block_header()));
    assert(blockHash.size() > 15);
    internal.mutable_raw_data()->set_ref_block_hash(blockHash.data() + 8, 8);

    const auto blockHeight = transaction.block_header().number();
    auto heightData = Data();
    encode64LE(blockHeight, heightData);
    std::reverse(heightData.begin(), heightData.end());
    internal.mutable_raw_data()->set_ref_block_bytes(heightData.data() + heightData.size() - 2, 2);
}

protocol::Transaction buildTransaction(const Proto::SigningInput& input) noexcept {
    auto tx = protocol::Transaction();

    if (input.transaction().has_transfer()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_TransferContract);

        auto transfer = to_internal(input.transaction().transfer());
        google::protobuf::Any any;
        any.PackFrom(transfer);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_transfer_asset()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_TransferAssetContract);

        auto transfer = to_internal(input.transaction().transfer_asset());
        google::protobuf::Any any;
        any.PackFrom(transfer);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_freeze_balance()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_FreezeBalanceContract);

        auto freeze_balance = to_internal(input.transaction().freeze_balance());
        google::protobuf::Any any;
        any.PackFrom(freeze_balance);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_freeze_balance_v2()) {
        auto* contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_FreezeBalanceV2Contract);
        auto freeze_balance = to_internal(input.transaction().freeze_balance_v2());
        google::protobuf::Any any;
        any.PackFrom(freeze_balance);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_unfreeze_balance()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_UnfreezeBalanceContract);
        auto unfreeze_balance = to_internal(input.transaction().unfreeze_balance());
        google::protobuf::Any any;
        any.PackFrom(unfreeze_balance);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_unfreeze_balance_v2()) {
        auto* contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_UnfreezeBalanceV2Contract);
        auto unfreeze_balance = to_internal(input.transaction().unfreeze_balance_v2());
        google::protobuf::Any any;
        any.PackFrom(unfreeze_balance);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_withdraw_expire_unfreeze()) {
        auto* contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_WithdrawExpireUnfreezeContract);
        auto withdraw_expire_unfreeze = to_internal(input.transaction().withdraw_expire_unfreeze());
        google::protobuf::Any any;
        any.PackFrom(withdraw_expire_unfreeze);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_delegate_resource()) {
        auto* contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_DelegateResourceContract);
        auto delegate_resource = to_internal(input.transaction().delegate_resource());
        google::protobuf::Any any;
        any.PackFrom(delegate_resource);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_undelegate_resource()) {
        auto* contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_UnDelegateResourceContract);
        auto undelegate_resource = to_internal(input.transaction().undelegate_resource());
        google::protobuf::Any any;
        any.PackFrom(undelegate_resource);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_unfreeze_asset()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_UnfreezeAssetContract);

        auto unfreeze_asset = to_internal(input.transaction().unfreeze_asset());
        google::protobuf::Any any;
        any.PackFrom(unfreeze_asset);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_vote_asset()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_VoteAssetContract);

        auto vote_asset = to_internal(input.transaction().vote_asset());
        google::protobuf::Any any;
        any.PackFrom(vote_asset);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_vote_witness()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_VoteWitnessContract);

        auto vote_witness = to_internal(input.transaction().vote_witness());
        google::protobuf::Any any;
        any.PackFrom(vote_witness);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_withdraw_balance()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_WithdrawBalanceContract);

        auto withdraw = to_internal(input.transaction().withdraw_balance());
        google::protobuf::Any any;
        any.PackFrom(withdraw);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_trigger_smart_contract()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_TriggerSmartContract);

        auto trigger_smart_contract = to_internal(input.transaction().trigger_smart_contract());
        google::protobuf::Any any;
        any.PackFrom(trigger_smart_contract);
        *contract->mutable_parameter() = any;
    } else if (input.transaction().has_transfer_trc20_contract()) {
        auto contract = tx.mutable_raw_data()->add_contract();
        contract->set_type(protocol::Transaction_Contract_ContractType_TriggerSmartContract);

        auto trigger_smart_contract = to_internal(input.transaction().transfer_trc20_contract());
        google::protobuf::Any any;
        any.PackFrom(trigger_smart_contract);
        *contract->mutable_parameter() = any;
    }

    tx.mutable_raw_data()->set_timestamp(input.transaction().timestamp());
    tx.mutable_raw_data()->set_expiration(input.transaction().expiration());
    tx.mutable_raw_data()->set_fee_limit(input.transaction().fee_limit());
    setBlockReference(input.transaction(), tx);

    return tx;
}

Data serialize(const protocol::Transaction& tx) noexcept {
    const auto serialized = tx.raw_data().SerializeAsString();
    return Data(serialized.begin(), serialized.end());
}

Proto::SigningOutput signDirect(const Proto::SigningInput& input) {
    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto hash = parse_hex(input.txid());
    const auto signature = key.sign(hash, TWCurveSECP256k1);
    auto output = Proto::SigningOutput();
    output.set_signature(signature.data(), signature.size());
    output.set_id(input.txid());
    output.set_id(hash.data(), hash.size());
    return output;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    if (!input.txid().empty()) {
        return signDirect(input);
    }

    auto output = Proto::SigningOutput();
    auto tx = buildTransaction(input);

    // Get default timestamp and expiration
    const uint64_t now = duration_cast<std::chrono::milliseconds>(
                             std::chrono::system_clock::now().time_since_epoch())
                             .count();
    const uint64_t timestamp = input.transaction().timestamp() == 0
                                   ? now
                                   : input.transaction().timestamp();
    const uint64_t expiration = input.transaction().expiration() == 0
                                    ? timestamp + 10 * 60 * 60 * 1000 // 10 hours
                                    : input.transaction().expiration();

    tx.mutable_raw_data()->set_timestamp(timestamp);
    tx.mutable_raw_data()->set_expiration(expiration);

    output.set_ref_block_bytes(tx.raw_data().ref_block_bytes());
    output.set_ref_block_hash(tx.raw_data().ref_block_hash());

    const auto hash = Hash::sha256(serialize(tx));

    const auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    const auto signature = key.sign(hash, TWCurveSECP256k1);

    const auto json = transactionJSON(tx, hash, signature).dump();

    output.set_id(hash.data(), hash.size());
    output.set_signature(signature.data(), signature.size());
    output.set_json(json.data(), json.size());

    return output;
}

Proto::SigningOutput Signer::compile(const Data& signature) const {
    Proto::SigningOutput output;
    auto preImage = signaturePreimage();
    auto hash = Hash::sha256(preImage);
    auto transaction = buildTransaction(input);
    const auto json = transactionJSON(transaction, hash, signature).dump();
    output.set_json(json.data(), json.size());
    output.set_ref_block_bytes(transaction.raw_data().ref_block_bytes());
    output.set_ref_block_hash(transaction.raw_data().ref_block_hash());
    output.set_id(hash.data(), hash.size());
    output.set_signature(signature.data(), signature.size());
    return output;
}

Data Signer::signaturePreimage() const {
    return serialize(buildTransaction(input));
}

} // namespace TW::Tron
