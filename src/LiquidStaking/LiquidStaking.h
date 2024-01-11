// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "proto/LiquidStaking.pb.h"
#include "TrustWalletCore/TWBlockchain.h"
#include <variant>
#include <optional>

namespace TW::LiquidStaking {
using TAction = std::variant<Proto::Stake, Proto::Unstake, Proto::Withdraw>;

enum class Action {
    Stake = 0,
    Unstake = 1,
    Withdraw = 2
};

class Builder {
    TAction mAction;
    std::string mFromAddress;
    std::optional<std::string> mSmartContractAddress{std::nullopt};
    Proto::Protocol mProtocol;
    Proto::Blockchain mBlockchain;

    Proto::Output buildStraderEVM() const;
    Proto::Output buildStrader() const;
    Proto::Output buildTortugaAptos() const;
    Proto::Output buildTortuga() const;
    Proto::Output buildStride() const;
    Proto::Output buildLidoEVM() const;
    Proto::Output buildLido() const;
public:
    Builder() noexcept = default;

    static Builder builder() noexcept { return {}; }

    Builder& protocol(Proto::Protocol protocol) noexcept {
        mProtocol = protocol;
        return *this;
    }

    Builder& blockchain(Proto::Blockchain blockchain) noexcept {
        mBlockchain = blockchain;
        return *this;
    }

    Builder& action(TAction action) noexcept {
        mAction = std::move(action);
        return *this;
    }

    Builder& smartContractAddress(std::string smartContractAddress) noexcept {
        if (!smartContractAddress.empty()) {
            mSmartContractAddress = std::move(smartContractAddress);
        }
        return *this;
    }

    Proto::Output build() const;
};

static inline Proto::Status generateError(Proto::StatusCode code, const std::optional<std::string>& message = std::nullopt) {
    Proto::Status status;
    status.set_code(code);
    switch (code) {
    case Proto::ERROR_ACTION_NOT_SET:
        status.set_message(message.value_or("Liquid staking action not set"));
        break;
    case Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL:
        status.set_message(message.value_or("The selected protocol doesn't support the targeted blockchain"));
        break;
    case Proto::ERROR_SMART_CONTRACT_ADDRESS_NOT_SET:
        status.set_message(message.value_or("The selected protocol require a smart contract address to be set"));
        break;
    case Proto::ERROR_INPUT_PROTO_DESERIALIZATION:
        status.set_message(message.value_or("Could not deserialize input proto"));
        break;
    case Proto::ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL:
        status.set_message(message.value_or("The selected protocol doesn't support this liquid staking operation"));
        break;
    default:
        return status;
    }
    return status;
}

static inline Proto::Output build(const Proto::Input& input) {
    TAction action;
    switch (input.action_case()) {
    case Proto::Input::kStake:
        action = input.stake();
        break;
    case Proto::Input::kUnstake:
        action = input.unstake();
        break;
    case Proto::Input::kWithdraw:
        action = input.withdraw();
        break;
    default:
        auto output = Proto::Output();
        *output.mutable_status() = generateError(Proto::ERROR_ACTION_NOT_SET);
        return output;
    }

    return Builder::builder().action(action).protocol(input.protocol()).smartContractAddress(input.smart_contract_address()).blockchain(input.blockchain()).build();
}
} // namespace TW::LiquidStaking
