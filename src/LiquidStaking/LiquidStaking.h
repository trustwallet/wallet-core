// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "proto/LiquidStaking.pb.h"
#include "TrustWalletCore/TWBlockchain.h"
#include <variant>

namespace TW::LiquidStaking {
using TAction = std::variant<Proto::Stake, Proto::Unstake, Proto::Withdraw>;

class Builder {
    TAction mAction;
    std::optional<std::string> mSmartContractAddress{std::nullopt};
    Proto::Protocol mProtocol;
    Proto::Blockchain mBlockchain;

    Proto::LiquidStakingOutput buildStraderEVM() const;
    Proto::LiquidStakingOutput buildStrader() const;
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

    Proto::LiquidStakingOutput build() const;
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
    return Builder::builder().action(action).protocol(input.protocol()).smartContractAddress(input.smart_contract_address()).blockchain(input.blockchain()).build();
}
} // namespace TW::LiquidStaking
