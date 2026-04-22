// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include "Address.h"

#include "Protobuf/TronInternal.pb.h"

#include "Deserialization.h"
#include "Serialization.h"
#include "../Base58.h"
#include "../BinaryCoding.h"
#include "../HexCoding.h"

#include <nlohmann/json.hpp>
#include <cassert>
#include <chrono>
#include <stdexcept>

namespace TW::Tron {

const std::string TRANSFER_TOKEN_FUNCTION = "0xa9059cbb";

/// Decode a Base58Check-encoded Tron address, throwing on invalid input.
static Data decodeAddress(const std::string& addr) {
    auto decoded = Base58::decodeCheck(addr);
    if (decoded.empty() || decoded.size() != Address::size) {
        throw std::invalid_argument("Invalid Tron address: " + addr);
    }
    if (decoded[0] != Address::prefix) {
        throw std::invalid_argument("Invalid Tron address: " + addr);
    }
    return decoded;
}

/// Converts an external TransferContract to an internal one used for signing.
protocol::TransferContract to_internal(const Proto::TransferContract& transfer) {
    auto internal = protocol::TransferContract();

    const auto ownerAddress = decodeAddress(transfer.owner_address());
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    const auto toAddress = decodeAddress(transfer.to_address());
    internal.set_to_address(toAddress.data(), toAddress.size());

    internal.set_amount(transfer.amount());

    return internal;
}

/// Converts an external TransferAssetContract to an internal one used for
/// signing.
protocol::TransferAssetContract to_internal(const Proto::TransferAssetContract& transfer) {
    auto internal = protocol::TransferAssetContract();

    internal.set_asset_name(transfer.asset_name());

    const auto ownerAddress = decodeAddress(transfer.owner_address());
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    const auto toAddress = decodeAddress(transfer.to_address());
    internal.set_to_address(toAddress.data(), toAddress.size());

    internal.set_amount(transfer.amount());

    return internal;
}

protocol::FreezeBalanceContract to_internal(const Proto::FreezeBalanceContract& freezeContract) {
    auto internal = protocol::FreezeBalanceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = decodeAddress(freezeContract.owner_address());
    const auto receiverAddress = decodeAddress(freezeContract.receiver_address());

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
    const auto ownerAddress = decodeAddress(freezeContract.owner_address());

    protocol::ResourceCode_Parse(freezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_frozen_balance(freezeContract.frozen_balance());

    return internal;
}

protocol::UnfreezeBalanceContract to_internal(const Proto::UnfreezeBalanceContract& unfreezeContract) {
    auto internal = protocol::UnfreezeBalanceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = decodeAddress(unfreezeContract.owner_address());
    const auto receiverAddress = decodeAddress(unfreezeContract.receiver_address());

    protocol::ResourceCode_Parse(unfreezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_receiver_address(receiverAddress.data(), receiverAddress.size());

    return internal;
}

protocol::UnfreezeBalanceV2Contract to_internal(const Proto::UnfreezeBalanceV2Contract& unfreezeContract) {
    auto internal = protocol::UnfreezeBalanceV2Contract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = decodeAddress(unfreezeContract.owner_address());

    protocol::ResourceCode_Parse(unfreezeContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_unfreeze_balance(unfreezeContract.unfreeze_balance());

    return internal;
}

protocol::DelegateResourceContract to_internal(const Proto::DelegateResourceContract& delegateContract) {
    auto internal = protocol::DelegateResourceContract();
    auto resource = protocol::ResourceCode();
    const auto ownerAddress = decodeAddress(delegateContract.owner_address());
    const auto receiverAddress = decodeAddress(delegateContract.receiver_address());

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
    const auto ownerAddress = decodeAddress(undelegateContract.owner_address());
    const auto receiverAddress = decodeAddress(undelegateContract.receiver_address());

    protocol::ResourceCode_Parse(undelegateContract.resource(), &resource);

    internal.set_resource(resource);
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_receiver_address(receiverAddress.data(), receiverAddress.size());
    internal.set_balance(undelegateContract.balance());

    return internal;
}

protocol::WithdrawExpireUnfreezeContract to_internal(const Proto::WithdrawExpireUnfreezeContract& withdrawExpireUnfreezeContract) {
    auto internal = protocol::WithdrawExpireUnfreezeContract();
    const auto ownerAddress = decodeAddress(withdrawExpireUnfreezeContract.owner_address());
    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    return internal;
}

protocol::UnfreezeAssetContract to_internal(const Proto::UnfreezeAssetContract& unfreezeContract) {
    auto internal = protocol::UnfreezeAssetContract();
    const auto ownerAddress = decodeAddress(unfreezeContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    return internal;
}

protocol::VoteAssetContract to_internal(const Proto::VoteAssetContract& voteContract) {
    auto internal = protocol::VoteAssetContract();
    const auto ownerAddress = decodeAddress(voteContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_support(voteContract.support());
    internal.set_count(voteContract.count());
    for (int i = 0; i < voteContract.vote_address_size(); i++) {
        auto voteAddress = decodeAddress(voteContract.vote_address(i));
        internal.add_vote_address(voteAddress.data(), voteAddress.size());
    }

    return internal;
}

protocol::VoteWitnessContract to_internal(const Proto::VoteWitnessContract& voteContract) {
    auto internal = protocol::VoteWitnessContract();
    const auto ownerAddress = decodeAddress(voteContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_support(voteContract.support());
    for (int i = 0; i < voteContract.votes_size(); i++) {
        auto voteAddress = decodeAddress(voteContract.votes(i).vote_address());
        auto* vote = internal.add_votes();

        vote->set_vote_address(voteAddress.data(), voteAddress.size());
        vote->set_vote_count(voteContract.votes(i).vote_count());
    }

    return internal;
}

protocol::WithdrawBalanceContract to_internal(const Proto::WithdrawBalanceContract& withdrawContract) {
    auto internal = protocol::WithdrawBalanceContract();
    const auto ownerAddress = decodeAddress(withdrawContract.owner_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());

    return internal;
}

protocol::TriggerSmartContract to_internal(const Proto::TriggerSmartContract& triggerSmartContract) {
    auto internal = protocol::TriggerSmartContract();
    const auto ownerAddress = decodeAddress(triggerSmartContract.owner_address());
    const auto contractAddress = decodeAddress(triggerSmartContract.contract_address());

    internal.set_owner_address(ownerAddress.data(), ownerAddress.size());
    internal.set_contract_address(contractAddress.data(), contractAddress.size());
    internal.set_call_value(triggerSmartContract.call_value());
    internal.set_data(triggerSmartContract.data().data(), triggerSmartContract.data().size());
    internal.set_call_token_value(triggerSmartContract.call_token_value());
    internal.set_token_id(triggerSmartContract.token_id());

    return internal;
}

protocol::TriggerSmartContract to_internal(const Proto::TransferTRC20Contract& transferTrc20Contract) {
    auto toAddress = decodeAddress(transferTrc20Contract.to_address());
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

protocol::Transaction buildTransaction(const Proto::SigningInput& input) {
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

    if (!input.transaction().memo().empty()) {
        tx.mutable_raw_data()->set_data(input.transaction().memo());
    }

    tx.mutable_raw_data()->set_timestamp(input.transaction().timestamp());
    tx.mutable_raw_data()->set_expiration(input.transaction().expiration());
    tx.mutable_raw_data()->set_fee_limit(input.transaction().fee_limit());
    setBlockReference(input.transaction(), tx);

    return tx;
}

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    if (!input.raw_json().empty()) {
        return signRawJson(input);
    }

    auto output = Proto::SigningOutput();

    protocol::Transaction tx;
    try {
        tx = buildTransaction(input);
    } catch (const std::invalid_argument& e) {
        output.set_error(Common::Proto::Error_invalid_address);
        output.set_error_message(e.what());
        return output;
    }

    // Validate that a contract is present
    if (tx.raw_data().contract_size() == 0) {
        output.set_error(Common::Proto::Error_invalid_params);
        output.set_error_message("No supported contract is set");
        return output;
    }

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

    const auto hash = Hash::sha256(serializeTxRawData(tx));

    const auto key = PrivateKey(input.private_key(), TWCurveSECP256k1);
    const auto signature = key.sign(hash);

    const auto json = transactionJSON(tx, hash, signature).dump();

    output.set_id(hash.data(), hash.size());
    output.set_signature(signature.data(), signature.size());
    output.set_json(json.data(), json.size());

    return output;
}

Proto::SigningOutput errorOutput(const std::string& message, const Common::Proto::SigningError error = Common::Proto::Error_invalid_params) {
    Proto::SigningOutput output;
    output.set_error(error);
    output.set_error_message(message);
    return output;
}

Result<protocol::Transaction, Proto::SigningOutput> Signer::deserializeAndValidateRawJson(const std::string& rawJson) {
    using R = Result<protocol::Transaction, Proto::SigningOutput>;

    if (rawJson.size() > MAX_JSON_SIZE) {
        return R::failure(errorOutput("raw JSON exceeds maximum allowed size 1 MB"));
    }

    nlohmann::json parsed;
    try {
        parsed = nlohmann::json::parse(rawJson);
    } catch (const std::exception& e) {
        return R::failure(errorOutput(e.what()));
    }

    if (!parsed.contains("txID") || !parsed["txID"].is_string()) {
        return R::failure(errorOutput("No txID found in raw JSON"));
    }
    const auto txID = parse_hex(parsed["txID"].get<std::string>());

    // In some cases, `raw_data_hex` can be omitted in the raw JSON. Validate it if it's present only.
    if (parsed.contains("raw_data_hex")) {
        if (!parsed["raw_data_hex"].is_string()) {
            return R::failure(errorOutput("raw_data_hex is not a string in raw JSON"));
        }

        const auto rawDataBytes = parse_hex(parsed["raw_data_hex"].get<std::string>());
        const auto rawDataHash = Hash::sha256(rawDataBytes);
        if (txID != rawDataHash) {
            return R::failure(errorOutput("txID does not match hash of raw_data_hex", Common::Proto::Error_tx_hash_mismatch));
        }
    }

    const auto transaction = transactionFromJSON(rawJson);
    if (transaction.isFailure()) {
        const auto errorMessage = "`raw_json` could not be parsed, or the transaction type is not supported: " + transaction.error();
        return R::failure(errorOutput(errorMessage, Common::Proto::Error_not_supported));
    }

    // Validate that a contract is present
    if (transaction.payload().raw_data().contract_size() == 0) {
        return R::failure(errorOutput("No supported contract is set"));
    }

    const auto expectedTxID = Hash::sha256(serializeTxRawData(transaction.payload()));
    if (txID != expectedTxID) {
        return R::failure(errorOutput("txID does not match hash of rawJson transaction", Common::Proto::Error_tx_hash_mismatch));
    }

    return R::success(transaction.payload());
}

Proto::SigningOutput Signer::signRawJson(const Proto::SigningInput& input) {
    const auto result = deserializeAndValidateRawJson(input.raw_json());
    if (result.isFailure()) {
        return result.error();
    }

    const auto& transaction = result.payload();
    const auto expectedTxID = Hash::sha256(serializeTxRawData(transaction));
    const auto key = PrivateKey(input.private_key(), TWCurveSECP256k1);
    const auto signature = key.sign(expectedTxID);
    const auto json = transactionJSON(transaction, expectedTxID, signature).dump();

    Proto::SigningOutput output;
    output.set_ref_block_bytes(transaction.raw_data().ref_block_bytes());
    output.set_ref_block_hash(transaction.raw_data().ref_block_hash());
    output.set_id(expectedTxID.data(), expectedTxID.size());
    output.set_signature(signature.data(), signature.size());
    output.set_json(json.data(), json.size());

    return output;
}

Proto::SigningOutput Signer::compileRawJson(const std::string& rawJson, const Data& signature) {
    const auto result = deserializeAndValidateRawJson(rawJson);
    if (result.isFailure()) {
        return result.error();
    }

    const auto& transaction = result.payload();
    const auto txID = Hash::sha256(serializeTxRawData(transaction));

    // It's safe to parse the JSON because deserializeAndValidateRawJson already checks that the JSON can be parsed
    // and that the txID matches the transaction data.
    auto parsed = nlohmann::json::parse(rawJson);
    // Add signature to JSON and set to output
    parsed["signature"] = nlohmann::json::array({hex(signature)});

    Proto::SigningOutput output;
    output.set_id(txID.data(), txID.size());
    output.set_signature(signature.data(), signature.size());
    output.set_json(parsed.dump());
    output.set_ref_block_bytes(transaction.raw_data().ref_block_bytes());
    output.set_ref_block_hash(transaction.raw_data().ref_block_hash());
    return output;
}

Proto::SigningOutput Signer::compile(const Data& signature) const {
    Proto::SigningOutput output;
    if (!input.raw_json().empty()) {
        return compileRawJson(input.raw_json(), signature);
    }
    protocol::Transaction transaction;
    try {
        transaction = buildTransaction(input);
    } catch (const std::invalid_argument& e) {
        output.set_error(Common::Proto::Error_invalid_address);
        output.set_error_message(e.what());
        return output;
    }

    // Validate that a contract is present
    if (transaction.raw_data().contract_size() == 0) {
        output.set_error(Common::Proto::Error_invalid_params);
        output.set_error_message("No supported contract is set");
        return output;
    }

    const auto hash = Hash::sha256(serializeTxRawData(transaction));
    const auto json = transactionJSON(transaction, hash, signature).dump();
    output.set_json(json.data(), json.size());
    output.set_ref_block_bytes(transaction.raw_data().ref_block_bytes());
    output.set_ref_block_hash(transaction.raw_data().ref_block_hash());
    output.set_id(hash.data(), hash.size());
    output.set_signature(signature.data(), signature.size());
    return output;
}

Result<Data, Proto::SigningOutput> Signer::signaturePreimageRawJson(const std::string& rawJson) {
    using R = Result<Data, Proto::SigningOutput>;
    const auto result = deserializeAndValidateRawJson(rawJson);
    if (result.isFailure()) {
        return R::failure(result.error());
    }
    return R::success(serializeTxRawData(result.payload()));
}

TxCompiler::Proto::PreSigningOutput Signer::signaturePreimage() const {
    TxCompiler::Proto::PreSigningOutput output;

    if (!input.raw_json().empty()) {
        const auto result = signaturePreimageRawJson(input.raw_json());
        if (result.isFailure()) {
            const auto error = result.error();
            output.set_error(error.error());
            output.set_error_message(error.error_message());
            return output;
        }

        const auto rawData = result.payload();
        const auto hash = Hash::sha256(rawData);
        output.set_data(rawData.data(), rawData.size());
        output.set_data_hash(hash.data(), hash.size());
        return output;
    }

    const auto rawData = serializeTxRawData(buildTransaction(input));
    const auto hash = Hash::sha256(rawData);
    output.set_data(rawData.data(), rawData.size());
    output.set_data_hash(hash.data(), hash.size());
    return output;
}

} // namespace TW::Tron
