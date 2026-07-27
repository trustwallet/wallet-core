// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Serialization.h"
#include "Constants.h"
#include "../HexCoding.h"
#include <sstream>
#include <vector>

namespace TW::Tron {

using json = nlohmann::json;

std::string typeName(const protocol::Transaction::Contract::ContractType type) {
    return protocol::Transaction::Contract::ContractType_Name(type);
}

std::string typeUrl(const protocol::Transaction::Contract::ContractType type) {
    std::ostringstream stringStream;
    stringStream << kFieldTypeUrlPrefix << "/protocol." << typeName(type);
    return stringStream.str();
}

json valueJSON(const protocol::TransferContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldToAddress] = hex(contract.to_address());
    valueJSON[kFieldAmount] = contract.amount();
    return valueJSON;
}

json valueJSON(const protocol::TransferAssetContract& contract) {
    json valueJSON;
    valueJSON[kFieldAssetName] = hex(contract.asset_name());
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldToAddress] = hex(contract.to_address());
    valueJSON[kFieldAmount] = contract.amount();
    return valueJSON;
}

json valueJSON(const protocol::VoteAssetContract& contract) {
    json valueJSON;

    std::vector<std::string> vote_address;
    for (const std::string& addr : contract.vote_address()) {
        vote_address.push_back(hex(addr));
    }

    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldVoteAddress] = vote_address;
    valueJSON[kFieldSupport] = contract.support();
    valueJSON[kFieldCount] = contract.count();

    return valueJSON;
}

json voteJSON(const protocol::VoteWitnessContract::Vote& vote) {
    json voteJSON;
    voteJSON[kFieldVoteAddress] = hex(vote.vote_address());
    voteJSON[kFieldVoteCount] = vote.vote_count();
    return voteJSON;
}

json valueJSON(const protocol::VoteWitnessContract& contract) {
    json valueJSON;

    std::vector<json> votes;
    for (const protocol::VoteWitnessContract::Vote& vote : contract.votes()) {
        votes.push_back(voteJSON(vote));
    }

    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldVotes] = votes;
    valueJSON[kFieldSupport] = contract.support();

    return valueJSON;
}

json valueJSON(const protocol::FreezeBalanceContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldFrozenBalance] = contract.frozen_balance();
    valueJSON[kFieldFrozenDuration] = contract.frozen_duration();
    valueJSON[kFieldResource] = protocol::ResourceCode_Name(contract.resource());
    valueJSON[kFieldReceiverAddress] = hex(contract.receiver_address());
    return valueJSON;
}

json valueJSON(const protocol::FreezeBalanceV2Contract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldFrozenBalance] = contract.frozen_balance();
    valueJSON[kFieldResource] = protocol::ResourceCode_Name(contract.resource());
    return valueJSON;
}

json valueJSON(const protocol::UnfreezeBalanceContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldResource] = protocol::ResourceCode_Name(contract.resource());
    valueJSON[kFieldReceiverAddress] = hex(contract.receiver_address());
    return valueJSON;
}

json valueJSON(const protocol::UnfreezeBalanceV2Contract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldResource] = protocol::ResourceCode_Name(contract.resource());
    valueJSON[kFieldUnfreezeBalance] = contract.unfreeze_balance();
    return valueJSON;
}

json valueJSON(const protocol::DelegateResourceContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldReceiverAddress] = hex(contract.receiver_address());
    valueJSON[kFieldResource] = protocol::ResourceCode_Name(contract.resource());
    valueJSON[kFieldBalance] = contract.balance();
    valueJSON[kFieldLock] = contract.lock();
    if (contract.lock_period() != 0) {
        valueJSON[kFieldLockPeriod] = contract.lock_period();
    }
    return valueJSON;
}

json valueJSON(const protocol::UnDelegateResourceContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldReceiverAddress] = hex(contract.receiver_address());
    valueJSON[kFieldResource] = protocol::ResourceCode_Name(contract.resource());
    valueJSON[kFieldBalance] = contract.balance();
    return valueJSON;
}

json valueJSON(const protocol::WithdrawExpireUnfreezeContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    return valueJSON;
}

json valueJSON(const protocol::WithdrawBalanceContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    return valueJSON;
}

json valueJSON(const protocol::UnfreezeAssetContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    return valueJSON;
}

json valueJSON(const protocol::TriggerSmartContract& contract) {
    json valueJSON;
    valueJSON[kFieldOwnerAddress] = hex(contract.owner_address());
    valueJSON[kFieldContractAddress] = hex(contract.contract_address());
    valueJSON[kFieldData] = hex(contract.data());
    if (contract.call_value() > 0) {
        valueJSON[kFieldCallValue] = contract.call_value();
    }
    if (contract.call_token_value() > 0) {
        valueJSON[kFieldCallTokenValue] = contract.call_token_value();
    }
    if (contract.token_id() > 0) {
        valueJSON[kFieldTokenId] = contract.token_id();
    }

    return valueJSON;
}

json parameterJSON(const google::protobuf::Any& parameter, const protocol::Transaction::Contract::ContractType type) {
    json paramJSON;
    paramJSON[kFieldTypeUrl] = typeUrl(type);

    switch (type) {
    case protocol::Transaction::Contract::TransferContract: {
        protocol::TransferContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::TransferAssetContract: {
        protocol::TransferAssetContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::VoteAssetContract: {
        protocol::VoteAssetContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::VoteWitnessContract: {
        protocol::VoteWitnessContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::FreezeBalanceContract: {
        protocol::FreezeBalanceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::FreezeBalanceV2Contract: {
        protocol::FreezeBalanceV2Contract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::UnfreezeBalanceContract: {
        protocol::UnfreezeBalanceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::UnfreezeBalanceV2Contract: {
        protocol::UnfreezeBalanceV2Contract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::WithdrawExpireUnfreezeContract: {
        protocol::WithdrawExpireUnfreezeContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::DelegateResourceContract: {
        protocol::DelegateResourceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::UnDelegateResourceContract: {
        protocol::UnDelegateResourceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::WithdrawBalanceContract: {
        protocol::WithdrawBalanceContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::UnfreezeAssetContract: {
        protocol::UnfreezeAssetContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::TriggerSmartContract: {
        protocol::TriggerSmartContract contract;
        parameter.UnpackTo(&contract);
        paramJSON[kFieldValue] = valueJSON(contract);
        break;
    }
    case protocol::Transaction::Contract::AccountCreateContract:
    default:
        break;
    }

    return paramJSON;
}

json contractJSON(const protocol::Transaction::Contract& contract) {
    json contractJSON;
    contractJSON[kFieldType] = typeName(contract.type());
    contractJSON[kFieldParameter] = parameterJSON(contract.parameter(), contract.type());
    return contractJSON;
}

json raw_dataJSON(const protocol::Transaction::raw& raw) {
    json raw_dataJSON;

    raw_dataJSON[kFieldRefBlockBytes] = hex(raw.ref_block_bytes());
    raw_dataJSON[kFieldRefBlockHash] = hex(raw.ref_block_hash());
    if (raw.ref_block_num() > 0) {
        raw_dataJSON[kFieldRefBlockNum] = raw.ref_block_num();
    }
    if (raw.fee_limit() > 0) {
        raw_dataJSON[kFieldFeeLimit] = raw.fee_limit();
    }
    raw_dataJSON[kFieldTimestamp] = raw.timestamp();
    raw_dataJSON[kFieldExpiration] = raw.expiration();
    if (!raw.data().empty()) {
        raw_dataJSON[kFieldData] = hex(raw.data());
    }
    auto contracts = json::array();
    for (const auto& contract : raw.contract()) {
        contracts.push_back(contractJSON(contract));
    }
    raw_dataJSON[kFieldContract] = contracts;

    return raw_dataJSON;
}

Data serializeTxRawData(const protocol::Transaction& tx) noexcept {
    const auto serialized = tx.raw_data().SerializeAsString();
    return { serialized.begin(), serialized.end() };
}

json transactionJSON(const protocol::Transaction& transaction, const TW::Data& txID, const TW::Data& signature) {
    json transactionJSON;
    transactionJSON[kFieldRawData] = raw_dataJSON(transaction.raw_data());
    transactionJSON[kFieldRawDataHex] = hex(serializeTxRawData(transaction));
    transactionJSON[kFieldTxID] = hex(txID);
    transactionJSON[kFieldSignature] = json::array({hex(signature)});

    return transactionJSON;
}

} // namespace TW::Tron
