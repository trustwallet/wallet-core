// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "LiquidStaking/LiquidStaking.h"
#include "Data.h"

// Stride
#include "proto/Cosmos.pb.h"

// Aptos
#include "proto/Aptos.pb.h"

// ETH
#include "Ethereum/ABI/Function.h"
#include "Ethereum/Address.h"
#include "proto/Ethereum.pb.h"
#include "uint256.h"

namespace TW::LiquidStaking {

using BlockchainActionEnumPair = std::pair<Proto::Blockchain, Action>;

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};

using EVMLiquidStakingFunctionRegistry = std::unordered_map<BlockchainActionEnumPair, std::string, PairHash>;
using EVMLiquidStakingParamsRegistry = std::unordered_map<BlockchainActionEnumPair, std::string, PairHash>;
using EVMLiquidStakingRegistry = std::unordered_map<Proto::Protocol, EVMLiquidStakingFunctionRegistry>;

static const EVMLiquidStakingFunctionRegistry gStraderFunctionRegistry =
    {{std::make_pair(Proto::POLYGON, Action::Stake), "swapMaticForMaticXViaInstantPool"},
     {std::make_pair(Proto::POLYGON, Action::Unstake), "requestMaticXSwap"},
     {std::make_pair(Proto::POLYGON, Action::Withdraw), "claimMaticXSwap"},
     {std::make_pair(Proto::BNB_BSC, Action::Stake), "deposit"},
     {std::make_pair(Proto::BNB_BSC, Action::Unstake), "requestWithdraw"}
};

static const EVMLiquidStakingFunctionRegistry gLidoFunctionRegistry =
    {{std::make_pair(Proto::ETHEREUM, Action::Stake), "submit"},
};

static const EVMLiquidStakingRegistry gEVMLiquidStakingRegistry = {
    {Proto::Protocol::Strader, gStraderFunctionRegistry},
    {Proto::Protocol::Lido, gLidoFunctionRegistry},
};

namespace internal {
    void setTransferDataAndAmount(Ethereum::Proto::Transaction::ContractGeneric& transfer, const Data& payload, const uint256_t& amount) {
        transfer.set_data(payload.data(), payload.size());
        Data amountData = store(amount);
        transfer.set_amount(amountData.data(), amountData.size());
    }

    void handleStake(const Proto::Stake& stake, const Proto::Blockchain& blockchain, Data& payload, uint256_t& amount, const Proto::Protocol protocol) {
        Ethereum::ABI::BaseParams params;
        if (protocol == Proto::Lido) {
            params.emplace_back(std::make_shared<Ethereum::ABI::ProtoAddress>());
        }
        auto funcData = Ethereum::ABI::Function::encodeFunctionCall(gEVMLiquidStakingRegistry.at(protocol).at({blockchain, Action::Stake}), params);
        if (funcData.has_value()) {
            payload = funcData.value();
        }
        amount = uint256_t(stake.amount());
    }

    void handleUnstake(const Proto::Unstake& unstake, const Proto::Blockchain& blockchain, Data& payload) {
        Ethereum::ABI::BaseParams params;
        params.emplace_back(std::make_shared<Ethereum::ABI::ProtoUInt256>(uint256_t(unstake.amount())));
        auto functionName = gStraderFunctionRegistry.at({blockchain, Action::Unstake});
        auto funcData = Ethereum::ABI::Function::encodeFunctionCall(functionName, params);
        if (funcData.has_value()) {
            payload = funcData.value();
        }
    }

    void handleWithdraw(const Proto::Withdraw& withdraw, const Proto::Blockchain& blockchain, Data& payload) {
        Ethereum::ABI::BaseParams params;
        params.emplace_back(std::make_shared<Ethereum::ABI::ProtoUInt256>(uint256_t(withdraw.idx())));
        auto functionName = gStraderFunctionRegistry.at({blockchain, Action::Withdraw});
        auto funcData = Ethereum::ABI::Function::encodeFunctionCall(functionName, params);
        if (funcData.has_value()) {
            payload = funcData.value();
        }
    }
}

Proto::Output Builder::buildStraderEVM() const {
    Proto::Output output;
    if (!mSmartContractAddress) {
        *output.mutable_status() = generateError(Proto::ERROR_SMART_CONTRACT_ADDRESS_NOT_SET, "Strader protocol require the smart contract address to be set");
        return output;
    }
    auto input = Ethereum::Proto::SigningInput();

    if (this->mBlockchain == Proto::POLYGON || this->mBlockchain == Proto::ETHEREUM) {
        input.set_tx_mode(Ethereum::Proto::Enveloped);
    }
    input.set_to_address(*mSmartContractAddress);

    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();

    auto visitFunctor = [&transfer, this](const TAction& value) {
        Data payload;
        uint256_t amount;

        if (auto* stake = std::get_if<Proto::Stake>(&value); stake) {
            internal::handleStake(*stake, mBlockchain, payload, amount, Proto::Protocol::Strader);
        } else if (auto* unstake = std::get_if<Proto::Unstake>(&value); unstake) {
            internal::handleUnstake(*unstake, mBlockchain, payload);
            amount = uint256_t(0);
        } else if (auto* withdraw = std::get_if<Proto::Withdraw>(&value); withdraw) {
            internal::handleWithdraw(*withdraw, mBlockchain, payload);
            amount = uint256_t(0);
        }

        internal::setTransferDataAndAmount(transfer, payload, amount);
    };

    std::visit(visitFunctor, this->mAction);

    *output.mutable_ethereum() = input;
    return output;
}

Proto::Output Builder::buildTortugaAptos() const {
    Proto::Output output;
    if (!mSmartContractAddress) {
        *output.mutable_status() = generateError(Proto::ERROR_SMART_CONTRACT_ADDRESS_NOT_SET, "Tortuga protocol require the smart contract address to be set");
        return output;
    }
    auto input = Aptos::Proto::SigningInput();
    auto &liquid_staking_message = *input.mutable_liquid_staking_message();
    liquid_staking_message.set_smart_contract_address(*mSmartContractAddress);

    auto visitFunctor = [&liquid_staking_message](const TAction& value) {
        if (auto* stake = std::get_if<Proto::Stake>(&value); stake) {
            auto& tortuga_stake = *liquid_staking_message.mutable_stake();
            tortuga_stake.set_amount(std::strtoull(stake->amount().c_str(), nullptr, 0));
        } else if (auto* unstake = std::get_if<Proto::Unstake>(&value); unstake) {
            auto& tortuga_unstake = *liquid_staking_message.mutable_unstake();
            tortuga_unstake.set_amount(std::strtoull(unstake->amount().c_str(), nullptr, 0));
        } else if (auto* withdraw = std::get_if<Proto::Withdraw>(&value); withdraw) {
            auto& tortuga_claim = *liquid_staking_message.mutable_claim();
            tortuga_claim.set_idx(std::strtoull(withdraw->idx().c_str(), nullptr, 0));
        }
    };

    std::visit(visitFunctor, this->mAction);
    *output.mutable_aptos() = input;
    return output;
}

Proto::Output Builder::buildStride() const {
    if (this->mBlockchain != Proto::STRIDE) {
        auto output = Proto::Output();
        *output.mutable_status() = generateError(Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL, "Only Stride blockchain is supported on stride for now");
        return output;
    }

    Proto::Output output;
    auto input = Cosmos::Proto::SigningInput();
    auto visitFunctor = [&input, &output](const TAction& value) {
        if (auto* stake = std::get_if<Proto::Stake>(&value); stake) {
            auto& stride_stake = *input.add_messages()->mutable_msg_stride_liquid_staking_stake();
            stride_stake.set_creator(stake->asset().from_address());
            stride_stake.set_amount(stake->amount());
            stride_stake.set_host_denom(stake->asset().denom());
        } else if (auto* unstake = std::get_if<Proto::Unstake>(&value); unstake) {
            auto& stride_redeem = *input.add_messages()->mutable_msg_stride_liquid_staking_redeem();
            stride_redeem.set_creator(unstake->asset().from_address());
            stride_redeem.set_amount(unstake->amount());
            stride_redeem.set_host_zone(unstake->receiver_chain_id());
            stride_redeem.set_receiver(unstake->receiver_address());
        } else {
            *output.mutable_status() = generateError(Proto::ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL, "Stride protocol unstake include withdraw operation");
        }
    };

    std::visit(visitFunctor, this->mAction);
    *output.mutable_cosmos() = input;
    return output;
}

Proto::Output Builder::buildTortuga() const {
    if (this->mBlockchain == Proto::APTOS) {
        return buildTortugaAptos();
    }
    auto output = Proto::Output();
    *output.mutable_status() = generateError(Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL, "Only Aptos blockchain is supported on tortuga for now");
    return output;
}

Proto::Output Builder::buildLidoEVM() const {
    Proto::Output output;
    if (!mSmartContractAddress) {
        *output.mutable_status() = generateError(Proto::ERROR_SMART_CONTRACT_ADDRESS_NOT_SET, "Lido protocol require the smart contract address to be set");
        return output;
    }
    auto input = Ethereum::Proto::SigningInput();

    if (this->mBlockchain == Proto::ETHEREUM) {
        input.set_tx_mode(Ethereum::Proto::Enveloped);
    }
    input.set_to_address(*mSmartContractAddress);

    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();


    auto visitFunctor = [&transfer, this, &output](const TAction& value) {
        Data payload;
        uint256_t amount;

        if (auto* stake = std::get_if<Proto::Stake>(&value); stake) {
            internal::handleStake(*stake, mBlockchain, payload, amount, Proto::Lido);
        } else {
            *output.mutable_status() = generateError(Proto::ERROR_OPERATION_NOT_SUPPORTED_BY_PROTOCOL, "Lido protocol only support stake action for now");
        }

        internal::setTransferDataAndAmount(transfer, payload, amount);
    };

    std::visit(visitFunctor, this->mAction);
    *output.mutable_ethereum() = input;
    return output;
}

Proto::Output Builder::buildLido() const {
    switch (this->mBlockchain) {
    case Proto::ETHEREUM:
        return buildLidoEVM();
    default:
        auto output = Proto::Output();
        *output.mutable_status() = generateError(Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL, "Only Lido EVM chains is supported for now");
        return output;
    }
}


Proto::Output Builder::buildStrader() const {
    switch (this->mBlockchain) {
    case Proto::POLYGON:
    case Proto::BNB_BSC:
        return buildStraderEVM();
    default:
        auto output = Proto::Output();
        *output.mutable_status() = generateError(Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL, "Only Strader EVM chains is supported for now");
        return output;
    }
}

Proto::Output Builder::build() const {
    switch (this->mProtocol) {
    case Proto::Strader:
        return this->buildStrader();
    case Proto::Tortuga:
        return this->buildTortuga();
    case Proto::Stride:
        return this->buildStride();
    case Proto::Lido:
        return this->buildLido();
    default:
        return Proto::Output();
    }
}

} // namespace TW::LiquidStaking
