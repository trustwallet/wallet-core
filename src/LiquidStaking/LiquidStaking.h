// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/LiquidStaking.pb.h"
#include <variant>

namespace TW::LiquidStaking {
using TAction = std::variant<Proto::Stake, Proto::Unstake, Proto::Withdraw>;

class Builder {
    TAction mAction;
    std::optional<std::string> mSmartContractAddress{std::nullopt};
    Proto::Protocol mProtocol;

public:
    Builder() noexcept = default;

    static Builder builder() noexcept { return {}; }

    Builder& protocol(Proto::Protocol protocol) noexcept {
        mProtocol = protocol;
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

    Proto::LiquidStakingOutput build() const;
};

static inline Proto::Status generateError(Proto::StatusCode code) {
    Proto::Status status;
    status.set_code(code);
    switch (code) {
    case Proto::ERROR_ACTION_NOT_SET:
        status.set_message("Liquid staking action not set");
        break;
    default:
        return status;
    }
    return status;
}

static inline Proto::LiquidStakingOutput build(const Proto::LiquidStakingInput& input) {
    TAction action;
    switch (input.action_case()) {
    case Proto::LiquidStakingInput::kStake:
        action = input.stake();
        break;
    case Proto::LiquidStakingInput::kUnstake:
        action = input.unstake();
        break;
    case Proto::LiquidStakingInput::kWithdraw:
        action = input.withdraw();
        break;
    default: {
        auto output = Proto::LiquidStakingOutput();
        *output.mutable_status() = generateError(Proto::ERROR_ACTION_NOT_SET);
        return output;
    }
    }
    return Builder::builder().action(action).protocol(input.protocol()).smartContractAddress(input.smart_contract_address()).build();
}
} // namespace TW::LiquidStaking
