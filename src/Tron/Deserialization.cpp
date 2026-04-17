// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Deserialization.h"
#include "../HexCoding.h"
#include "../Result.h"
#include <google/protobuf/any.pb.h>

namespace TW::Tron {

using json = nlohmann::json;
using VoidResult = Result<void>;

// Helper to parse a hex-encoded string field from JSON, returning an empty string if the field is missing.
// Throws an exception if the field is not a string or an invalid hex string.
std::string hexStringOrEmpty(const json& j, const std::string& key) {
    if (!j.contains(key)) {
        return {};
    }
    if (!j[key].is_string()) {
        throw std::runtime_error("Expected hex string for field '" + key + "'");
    }
    const auto bytesHex = j[key].get<std::string>();
    if (bytesHex.empty()) {
        return {};
    }
    const auto bytes = parse_hex(bytesHex);
    if (bytes.empty()) {
        throw std::runtime_error("Invalid hex string for field '" + key + "': " + bytesHex);
    }
    return {bytes.begin(), bytes.end()};
}

// ---------------------------------------------------------------------------
// Contract value deserializers
// ---------------------------------------------------------------------------

VoidResult fillTransferContract(const json& value, protocol::TransferContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    out.set_to_address(hexStringOrEmpty(value, "to_address"));
    if (value.contains("amount")) {
        out.set_amount(value["amount"].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillTransferAssetContract(const json& value, protocol::TransferAssetContract& out) {
    out.set_asset_name(hexStringOrEmpty(value, "asset_name"));
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    out.set_to_address(hexStringOrEmpty(value, "to_address"));
    if (value.contains("amount")) {
        out.set_amount(value["amount"].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillVoteAssetContract(const json& value, protocol::VoteAssetContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    if (value.contains("vote_address") && value["vote_address"].is_array()) {
        for (const auto& addr : value["vote_address"]) {
            const auto bytes = parse_hex(addr.get<std::string>());
            out.add_vote_address(bytes.data(), bytes.size());
        }
    }
    if (value.contains("support")) {
        out.set_support(value["support"].get<bool>());
    }
    if (value.contains("count")) {
        out.set_count(value["count"].get<int32_t>());
    }
    return VoidResult::success();
}

VoidResult fillVoteWitnessContract(const json& value, protocol::VoteWitnessContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    if (value.contains("support")) {
        out.set_support(value["support"].get<bool>());
    }
    if (value.contains("votes") && value["votes"].is_array()) {
        for (const auto& voteJ : value["votes"]) {
            auto* vote = out.add_votes();
            vote->set_vote_address(hexStringOrEmpty(voteJ, "vote_address"));
            if (voteJ.contains("vote_count")) {
                vote->set_vote_count(voteJ["vote_count"].get<int64_t>());
            }
        }
    }
    return VoidResult::success();
}

VoidResult fillFreezeBalanceContract(const json& value, protocol::FreezeBalanceContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    if (value.contains("frozen_balance")) {
        out.set_frozen_balance(value["frozen_balance"].get<int64_t>());
    }
    if (value.contains("frozen_duration")) {
        out.set_frozen_duration(value["frozen_duration"].get<int64_t>());
    }
    if (value.contains("resource")) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value["resource"].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    out.set_receiver_address(hexStringOrEmpty(value, "receiver_address"));
    return VoidResult::success();
}

VoidResult fillFreezeBalanceV2Contract(const json& value, protocol::FreezeBalanceV2Contract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    if (value.contains("frozen_balance")) {
        out.set_frozen_balance(value["frozen_balance"].get<int64_t>());
    }
    if (value.contains("resource")) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value["resource"].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    return VoidResult::success();
}

VoidResult fillUnfreezeBalanceContract(const json& value, protocol::UnfreezeBalanceContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    if (value.contains("resource")) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value["resource"].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    out.set_receiver_address(hexStringOrEmpty(value, "receiver_address"));
    return VoidResult::success();
}

VoidResult fillUnfreezeBalanceV2Contract(const json& value, protocol::UnfreezeBalanceV2Contract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    if (value.contains("unfreeze_balance")) {
        out.set_unfreeze_balance(value["unfreeze_balance"].get<int64_t>());
    }
    if (value.contains("resource")) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value["resource"].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    return VoidResult::success();
}

VoidResult fillWithdrawExpireUnfreezeContract(const json& value, protocol::WithdrawExpireUnfreezeContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    return VoidResult::success();
}

VoidResult fillDelegateResourceContract(const json& value, protocol::DelegateResourceContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    out.set_receiver_address(hexStringOrEmpty(value, "receiver_address"));
    if (value.contains("resource")) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value["resource"].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    if (value.contains("balance")) {
        out.set_balance(value["balance"].get<int64_t>());
    }
    if (value.contains("lock")) {
        out.set_lock(value["lock"].get<bool>());
    }
    return VoidResult::success();
}

VoidResult fillUnDelegateResourceContract(const json& value, protocol::UnDelegateResourceContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    out.set_receiver_address(hexStringOrEmpty(value, "receiver_address"));
    if (value.contains("resource")) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value["resource"].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    if (value.contains("balance")) {
        out.set_balance(value["balance"].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillWithdrawBalanceContract(const json& value, protocol::WithdrawBalanceContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    return VoidResult::success();
}

VoidResult fillUnfreezeAssetContract(const json& value, protocol::UnfreezeAssetContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    return VoidResult::success();
}

VoidResult fillTriggerSmartContract(const json& value, protocol::TriggerSmartContract& out) {
    out.set_owner_address(hexStringOrEmpty(value, "owner_address"));
    out.set_contract_address(hexStringOrEmpty(value, "contract_address"));
    out.set_data(hexStringOrEmpty(value, "data"));
    if (value.contains("call_value")) {
        out.set_call_value(value["call_value"].get<int64_t>());
    }
    if (value.contains("call_token_value")) {
        out.set_call_token_value(value["call_token_value"].get<int64_t>());
    }
    if (value.contains("token_id")) {
        out.set_token_id(value["token_id"].get<int64_t>());
    }
    return VoidResult::success();
}

// ---------------------------------------------------------------------------
// Contract dispatcher
// ---------------------------------------------------------------------------

VoidResult fillContract(const json& contractJ, protocol::Transaction::Contract& out) {
    if (!contractJ.contains("type") || !contractJ["type"].is_string()) {
        return VoidResult::failure("contract entry missing or invalid 'type' field");
    }
    if (!contractJ.contains("parameter") || !contractJ["parameter"].is_object()) {
        return VoidResult::failure("contract entry missing or invalid 'parameter' field");
    }

    const auto& param = contractJ["parameter"];
    const auto& value = param.contains("value") && param["value"].is_object()
                            ? param["value"]
                            : json::object();

    protocol::Transaction::Contract::ContractType type;
    if (!protocol::Transaction::Contract::ContractType_Parse(
            contractJ["type"].get<std::string>(), &type)) {
        return VoidResult::failure("unknown contract type: " + contractJ["type"].get<std::string>());
    }
    out.set_type(type);

    switch (type) {
    case protocol::Transaction::Contract::TransferContract: {
        protocol::TransferContract contract;
        auto result = fillTransferContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::TransferAssetContract: {
        protocol::TransferAssetContract contract;
        auto result = fillTransferAssetContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::VoteAssetContract: {
        protocol::VoteAssetContract contract;
        auto result = fillVoteAssetContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::VoteWitnessContract: {
        protocol::VoteWitnessContract contract;
        auto result = fillVoteWitnessContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::FreezeBalanceContract: {
        protocol::FreezeBalanceContract contract;
        auto result = fillFreezeBalanceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::FreezeBalanceV2Contract: {
        protocol::FreezeBalanceV2Contract contract;
        auto result = fillFreezeBalanceV2Contract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::UnfreezeBalanceContract: {
        protocol::UnfreezeBalanceContract contract;
        auto result = fillUnfreezeBalanceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::UnfreezeBalanceV2Contract: {
        protocol::UnfreezeBalanceV2Contract contract;
        auto result = fillUnfreezeBalanceV2Contract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::WithdrawExpireUnfreezeContract: {
        protocol::WithdrawExpireUnfreezeContract contract;
        auto result = fillWithdrawExpireUnfreezeContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::DelegateResourceContract: {
        protocol::DelegateResourceContract contract;
        auto result = fillDelegateResourceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::UnDelegateResourceContract: {
        protocol::UnDelegateResourceContract contract;
        auto result = fillUnDelegateResourceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::WithdrawBalanceContract: {
        protocol::WithdrawBalanceContract contract;
        auto result = fillWithdrawBalanceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::UnfreezeAssetContract: {
        protocol::UnfreezeAssetContract contract;
        auto result = fillUnfreezeAssetContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::TriggerSmartContract: {
        protocol::TriggerSmartContract contract;
        auto result = fillTriggerSmartContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, "type.googleapis.com");
        break;
    }
    case protocol::Transaction::Contract::AccountCreateContract:
    default:
        return VoidResult::failure("unsupported contract type: " + contractJ["type"].get<std::string>());
    }

    return VoidResult::success();
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

Result<protocol::Transaction> transactionFromJSONObject(const json& j) {
    using R = Result<protocol::Transaction>;
    try {
        if (!j.contains("raw_data") || !j["raw_data"].is_object()) {
            return R::failure("missing or invalid 'raw_data' field");
        }
        const auto& raw = j["raw_data"];

        protocol::Transaction tx;
        auto* rd = tx.mutable_raw_data();

        rd->set_ref_block_bytes(hexStringOrEmpty(raw, "ref_block_bytes"));
        rd->set_ref_block_hash(hexStringOrEmpty(raw, "ref_block_hash"));

        if (raw.contains("ref_block_num")) {
            rd->set_ref_block_num(raw["ref_block_num"].get<int64_t>());
        }
        if (raw.contains("timestamp")) {
            rd->set_timestamp(raw["timestamp"].get<int64_t>());
        }
        if (raw.contains("expiration")) {
            rd->set_expiration(raw["expiration"].get<int64_t>());
        }
        if (raw.contains("fee_limit")) {
            rd->set_fee_limit(raw["fee_limit"].get<int64_t>());
        }
        if (raw.contains("data") && raw["data"].is_string()) {
            rd->set_data(hexStringOrEmpty(raw, "data"));
        }

        if (!raw.contains("contract") || !raw["contract"].is_array() || raw["contract"].empty()) {
            return R::failure("missing or empty 'contract' array");
        }
        for (const auto& ctr : raw["contract"]) {
            auto result = fillContract(ctr, *rd->add_contract());
            if (result.isFailure()) {
                return R::failure(result.error());
            }
        }

        return R::success(std::move(tx));
    } catch (const std::exception& e) {
        return R::failure(e.what());
    } catch (...) {
        return R::failure("unknown error during JSON deserialization");
    }
}

Result<protocol::Transaction> transactionFromJSON(const std::string& jsonStr) {
    using R = Result<protocol::Transaction>;
    try {
        return transactionFromJSONObject(json::parse(jsonStr));
    } catch (const std::exception& e) {
        return R::failure(std::string("JSON parse error: ") + e.what());
    } catch (...) {
        return R::failure("unknown JSON parse error");
    }
}

} // namespace TW::Tron
