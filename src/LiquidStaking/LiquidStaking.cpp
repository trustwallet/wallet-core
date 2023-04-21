// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LiquidStaking/LiquidStaking.h"
#include "Data.h"

// ETH
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "Ethereum/ABI/ParamBase.h"
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

using StraderFunctionRegistry = std::unordered_map<BlockchainActionEnumPair, std::string, PairHash>;
using StraderParamsRegistry = std::unordered_map<BlockchainActionEnumPair, std::string, PairHash>;
using Params = std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>;

static const StraderFunctionRegistry gStraderFunctionRegistry =
    {{std::make_pair(Proto::POLYGON, Action::Stake), "swapMaticForMaticXViaInstantPool"},
     {std::make_pair(Proto::POLYGON, Action::Unstake), "requestMaticXSwap"},
     {std::make_pair(Proto::BNB_BSC, Action::Stake), "deposit"},
     {std::make_pair(Proto::BNB_BSC, Action::Unstake), "requestWithdraw"}
};

namespace internal {
    void setTransferDataAndAmount(Ethereum::Proto::Transaction::ContractGeneric& transfer, const Data& payload, const uint256_t& amount) {
        transfer.set_data(payload.data(), payload.size());
        Data amountData = store(amount);
        transfer.set_amount(amountData.data(), amountData.size());
    }

    void handleStake(const Proto::Stake& stake, const Proto::Blockchain& blockchain, Data& payload, uint256_t& amount) {
        auto func = Ethereum::ABI::Function(gStraderFunctionRegistry.at({blockchain, Action::Stake}));
        func.encode(payload);
        amount = uint256_t(stake.amount());
    }

    void handleUnstake(const Proto::Unstake& unstake, const Proto::Blockchain& blockchain, Data& payload) {
        Params params;
        params.emplace_back(std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(unstake.amount())));
        auto functionName = gStraderFunctionRegistry.at({blockchain, Action::Unstake});
        auto func = Ethereum::ABI::Function(functionName, params);
        func.encode(payload);
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
            internal::handleStake(*stake, mBlockchain, payload, amount);
        } else if (auto* unstake = std::get_if<Proto::Unstake>(&value); unstake) {
            internal::handleUnstake(*unstake, mBlockchain, payload);
            amount = uint256_t(0);
        }

        internal::setTransferDataAndAmount(transfer, payload, amount);
    };

    std::visit(visitFunctor, this->mAction);

    *output.mutable_ethereum() = input;
    return output;
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
    default:
        return Proto::Output();
    }
}

} // namespace TW::LiquidStaking
