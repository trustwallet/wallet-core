// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Deserialization.h"
#include "Constants.h"
#include "../HexCoding.h"
#include "../Result.h"
#include <google/protobuf/any.pb.h>
#include <unordered_set>

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

// Helper to parse a hex-encoded string field from JSON, falling back to the raw UTF-8 value if
// the string is not valid hex. Used for fields that are semantically human-readable labels
// (e.g. ContractName) but may be hex-encoded in some contexts.
std::string hexStringOrPlainText(const json& j, const std::string& key) {
    if (!j.contains(key)) {
        return {};
    }
    if (!j[key].is_string()) {
        throw std::runtime_error("Expected string for field '" + key + "'");
    }
    auto raw = j[key].get<std::string>();
    if (raw.empty()) {
        return {};
    }
    const auto bytes = parse_hex(raw);
    if (!bytes.empty()) {
        return {bytes.begin(), bytes.end()};
    }
    return raw;
}

VoidResult checkNoExtraFields(const json& value, const std::initializer_list<std::string>& allowed) {
    const std::unordered_set<std::string> allowedSet(allowed);
    for (const auto& [key, _] : value.items()) {
        if (allowedSet.find(key) == allowedSet.end()) {
            return VoidResult::failure("unexpected field '" + key + "' in contract value");
        }
    }
    return VoidResult::success();
}

// ---------------------------------------------------------------------------
// Contract value deserializers
// ---------------------------------------------------------------------------

VoidResult fillTransferContract(const json& value, protocol::TransferContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldToAddress, kFieldAmount});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    out.set_to_address(hexStringOrEmpty(value, kFieldToAddress));
    if (value.contains(kFieldAmount)) {
        out.set_amount(value[kFieldAmount].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillTransferAssetContract(const json& value, protocol::TransferAssetContract& out) {
    auto check = checkNoExtraFields(value, {kFieldAssetName, kFieldOwnerAddress, kFieldToAddress, kFieldAmount});
    if (check.isFailure()) { return check; }
    out.set_asset_name(hexStringOrEmpty(value, kFieldAssetName));
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    out.set_to_address(hexStringOrEmpty(value, kFieldToAddress));
    if (value.contains(kFieldAmount)) {
        out.set_amount(value[kFieldAmount].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillVoteAssetContract(const json& value, protocol::VoteAssetContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldVoteAddress, kFieldSupport, kFieldCount});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    if (value.contains(kFieldVoteAddress) && value[kFieldVoteAddress].is_array()) {
        for (const auto& addr : value[kFieldVoteAddress]) {
            const auto bytes = parse_hex(addr.get<std::string>());
            out.add_vote_address(bytes.data(), bytes.size());
        }
    }
    if (value.contains(kFieldSupport)) {
        out.set_support(value[kFieldSupport].get<bool>());
    }
    if (value.contains(kFieldCount)) {
        out.set_count(value[kFieldCount].get<int32_t>());
    }
    return VoidResult::success();
}

VoidResult fillVoteWitnessContract(const json& value, protocol::VoteWitnessContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldSupport, kFieldVotes});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    if (value.contains(kFieldSupport)) {
        out.set_support(value[kFieldSupport].get<bool>());
    }
    if (value.contains(kFieldVotes) && value[kFieldVotes].is_array()) {
        for (const auto& voteJ : value[kFieldVotes]) {
            auto voteCheck = checkNoExtraFields(voteJ, {kFieldVoteAddress, kFieldVoteCount});
            if (voteCheck.isFailure()) { return voteCheck; }
            auto* vote = out.add_votes();
            vote->set_vote_address(hexStringOrEmpty(voteJ, kFieldVoteAddress));
            if (voteJ.contains(kFieldVoteCount)) {
                vote->set_vote_count(voteJ[kFieldVoteCount].get<int64_t>());
            }
        }
    }
    return VoidResult::success();
}

VoidResult fillFreezeBalanceContract(const json& value, protocol::FreezeBalanceContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldFrozenBalance, kFieldFrozenDuration, kFieldResource, kFieldReceiverAddress});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    if (value.contains(kFieldFrozenBalance)) {
        out.set_frozen_balance(value[kFieldFrozenBalance].get<int64_t>());
    }
    if (value.contains(kFieldFrozenDuration)) {
        out.set_frozen_duration(value[kFieldFrozenDuration].get<int64_t>());
    }
    if (value.contains(kFieldResource)) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value[kFieldResource].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    out.set_receiver_address(hexStringOrEmpty(value, kFieldReceiverAddress));
    return VoidResult::success();
}

VoidResult fillFreezeBalanceV2Contract(const json& value, protocol::FreezeBalanceV2Contract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldFrozenBalance, kFieldResource});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    if (value.contains(kFieldFrozenBalance)) {
        out.set_frozen_balance(value[kFieldFrozenBalance].get<int64_t>());
    }
    if (value.contains(kFieldResource)) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value[kFieldResource].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    return VoidResult::success();
}

VoidResult fillUnfreezeBalanceContract(const json& value, protocol::UnfreezeBalanceContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldResource, kFieldReceiverAddress});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    if (value.contains(kFieldResource)) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value[kFieldResource].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    out.set_receiver_address(hexStringOrEmpty(value, kFieldReceiverAddress));
    return VoidResult::success();
}

VoidResult fillUnfreezeBalanceV2Contract(const json& value, protocol::UnfreezeBalanceV2Contract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldUnfreezeBalance, kFieldResource});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    if (value.contains(kFieldUnfreezeBalance)) {
        out.set_unfreeze_balance(value[kFieldUnfreezeBalance].get<int64_t>());
    }
    if (value.contains(kFieldResource)) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value[kFieldResource].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    return VoidResult::success();
}

VoidResult fillWithdrawExpireUnfreezeContract(const json& value, protocol::WithdrawExpireUnfreezeContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    return VoidResult::success();
}

VoidResult fillDelegateResourceContract(const json& value, protocol::DelegateResourceContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldReceiverAddress, kFieldResource, kFieldBalance, kFieldLock, kFieldLockPeriod});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    out.set_receiver_address(hexStringOrEmpty(value, kFieldReceiverAddress));
    if (value.contains(kFieldResource)) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value[kFieldResource].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    if (value.contains(kFieldBalance)) {
        out.set_balance(value[kFieldBalance].get<int64_t>());
    }
    if (value.contains(kFieldLock)) {
        out.set_lock(value[kFieldLock].get<bool>());
    }
    if (value.contains(kFieldLockPeriod)) {
        out.set_lock_period(value[kFieldLockPeriod].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillUnDelegateResourceContract(const json& value, protocol::UnDelegateResourceContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldReceiverAddress, kFieldResource, kFieldBalance});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    out.set_receiver_address(hexStringOrEmpty(value, kFieldReceiverAddress));
    if (value.contains(kFieldResource)) {
        protocol::ResourceCode code;
        if (protocol::ResourceCode_Parse(value[kFieldResource].get<std::string>(), &code)) {
            out.set_resource(code);
        }
    }
    if (value.contains(kFieldBalance)) {
        out.set_balance(value[kFieldBalance].get<int64_t>());
    }
    return VoidResult::success();
}

VoidResult fillWithdrawBalanceContract(const json& value, protocol::WithdrawBalanceContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    return VoidResult::success();
}

VoidResult fillUnfreezeAssetContract(const json& value, protocol::UnfreezeAssetContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    return VoidResult::success();
}

VoidResult fillTriggerSmartContract(const json& value, protocol::TriggerSmartContract& out) {
    auto check = checkNoExtraFields(value, {kFieldOwnerAddress, kFieldContractAddress, kFieldData, kFieldCallValue, kFieldCallTokenValue, kFieldTokenId});
    if (check.isFailure()) { return check; }
    out.set_owner_address(hexStringOrEmpty(value, kFieldOwnerAddress));
    out.set_contract_address(hexStringOrEmpty(value, kFieldContractAddress));
    out.set_data(hexStringOrEmpty(value, kFieldData));
    if (value.contains(kFieldCallValue)) {
        out.set_call_value(value[kFieldCallValue].get<int64_t>());
    }
    if (value.contains(kFieldCallTokenValue)) {
        out.set_call_token_value(value[kFieldCallTokenValue].get<int64_t>());
    }
    if (value.contains(kFieldTokenId)) {
        out.set_token_id(value[kFieldTokenId].get<int64_t>());
    }
    return VoidResult::success();
}

// ---------------------------------------------------------------------------
// Contract dispatcher
// ---------------------------------------------------------------------------

VoidResult fillContract(const json& contractJ, protocol::Transaction::Contract& out) {
    auto checkContract = checkNoExtraFields(contractJ, {kFieldType, kFieldParameter, kFieldProvider, kFieldContractName, kFieldPermId});
    if (checkContract.isFailure()) { return checkContract; }

    if (!contractJ.contains(kFieldType) || !contractJ[kFieldType].is_string()) {
        return VoidResult::failure("contract entry missing or invalid 'type' field");
    }
    if (!contractJ.contains(kFieldParameter) || !contractJ[kFieldParameter].is_object()) {
        return VoidResult::failure("contract entry missing or invalid 'parameter' field");
    }

    const auto& param = contractJ[kFieldParameter];
    auto checkParam = checkNoExtraFields(param, {kFieldValue, kFieldTypeUrl});
    if (checkParam.isFailure()) { return checkParam; }

    const auto& value = param.contains(kFieldValue) && param[kFieldValue].is_object()
                            ? param[kFieldValue]
                            : json::object();

    protocol::Transaction::Contract::ContractType type;
    if (!protocol::Transaction::Contract::ContractType_Parse(
            contractJ[kFieldType].get<std::string>(), &type)) {
        return VoidResult::failure("unknown contract type: " + contractJ[kFieldType].get<std::string>());
    }
    out.set_type(type);

    if (contractJ.contains(kFieldContractName) && contractJ[kFieldContractName].is_string()) {
        out.set_contractname(hexStringOrPlainText(contractJ, kFieldContractName));
    }
    if (contractJ.contains(kFieldPermId) && contractJ[kFieldPermId].is_number_integer()) {
        out.set_permission_id(contractJ[kFieldPermId].get<int32_t>());
    }

    switch (type) {
    case protocol::Transaction::Contract::TransferContract: {
        protocol::TransferContract contract;
        auto result = fillTransferContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::TransferAssetContract: {
        protocol::TransferAssetContract contract;
        auto result = fillTransferAssetContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::VoteAssetContract: {
        protocol::VoteAssetContract contract;
        auto result = fillVoteAssetContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::VoteWitnessContract: {
        protocol::VoteWitnessContract contract;
        auto result = fillVoteWitnessContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::FreezeBalanceContract: {
        protocol::FreezeBalanceContract contract;
        auto result = fillFreezeBalanceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::FreezeBalanceV2Contract: {
        protocol::FreezeBalanceV2Contract contract;
        auto result = fillFreezeBalanceV2Contract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::UnfreezeBalanceContract: {
        protocol::UnfreezeBalanceContract contract;
        auto result = fillUnfreezeBalanceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::UnfreezeBalanceV2Contract: {
        protocol::UnfreezeBalanceV2Contract contract;
        auto result = fillUnfreezeBalanceV2Contract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::WithdrawExpireUnfreezeContract: {
        protocol::WithdrawExpireUnfreezeContract contract;
        auto result = fillWithdrawExpireUnfreezeContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::DelegateResourceContract: {
        protocol::DelegateResourceContract contract;
        auto result = fillDelegateResourceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::UnDelegateResourceContract: {
        protocol::UnDelegateResourceContract contract;
        auto result = fillUnDelegateResourceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::WithdrawBalanceContract: {
        protocol::WithdrawBalanceContract contract;
        auto result = fillWithdrawBalanceContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::UnfreezeAssetContract: {
        protocol::UnfreezeAssetContract contract;
        auto result = fillUnfreezeAssetContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::TriggerSmartContract: {
        protocol::TriggerSmartContract contract;
        auto result = fillTriggerSmartContract(value, contract);
        if (result.isFailure()) { return result; }
        out.mutable_parameter()->PackFrom(contract, kFieldTypeUrlPrefix);
        break;
    }
    case protocol::Transaction::Contract::AccountCreateContract:
    default:
        return VoidResult::failure("unsupported contract type: " + contractJ[kFieldType].get<std::string>());
    }

    return VoidResult::success();
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

Result<protocol::Transaction> transactionFromJSONObject(const json& j) {
    using R = Result<protocol::Transaction>;
    try {
        if (!j.contains(kFieldRawData) || !j[kFieldRawData].is_object()) {
            return R::failure("missing or invalid 'raw_data' field");
        }
        const auto& raw = j[kFieldRawData];

        auto checkRaw = checkNoExtraFields(raw, {kFieldRefBlockBytes, kFieldRefBlockHash, kFieldRefBlockNum, kFieldTimestamp, kFieldExpiration, kFieldFeeLimit, kFieldData, kFieldContract});
        if (checkRaw.isFailure()) { return R::failure(checkRaw.error()); }

        protocol::Transaction tx;
        auto* rd = tx.mutable_raw_data();

        rd->set_ref_block_bytes(hexStringOrEmpty(raw, kFieldRefBlockBytes));
        rd->set_ref_block_hash(hexStringOrEmpty(raw, kFieldRefBlockHash));

        if (raw.contains(kFieldRefBlockNum)) {
            rd->set_ref_block_num(raw[kFieldRefBlockNum].get<int64_t>());
        }
        if (raw.contains(kFieldTimestamp)) {
            rd->set_timestamp(raw[kFieldTimestamp].get<int64_t>());
        }
        if (raw.contains(kFieldExpiration)) {
            rd->set_expiration(raw[kFieldExpiration].get<int64_t>());
        }
        if (raw.contains(kFieldFeeLimit)) {
            rd->set_fee_limit(raw[kFieldFeeLimit].get<int64_t>());
        }
        if (raw.contains(kFieldData) && raw[kFieldData].is_string()) {
            rd->set_data(hexStringOrEmpty(raw, kFieldData));
        }

        if (!raw.contains(kFieldContract) || !raw[kFieldContract].is_array() || raw[kFieldContract].empty()) {
            return R::failure("missing or empty 'contract' array");
        }
        for (const auto& contractJ : raw[kFieldContract]) {
            auto result = fillContract(contractJ, *rd->add_contract());
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

    if (jsonStr.size() > MAX_JSON_SIZE) {
        return R::failure("raw JSON exceeds maximum allowed size 1 MB");
    }

    try {
        return transactionFromJSONObject(json::parse(jsonStr));
    } catch (const std::exception& e) {
        return R::failure(std::string("JSON parse error: ") + e.what());
    } catch (...) {
        return R::failure("unknown JSON parse error");
    }
}

} // namespace TW::Tron
