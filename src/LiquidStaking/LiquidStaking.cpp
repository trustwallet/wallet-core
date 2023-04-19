// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LiquidStaking/LiquidStaking.h"
#include "Data.h"

// ETH
#include "proto/Ethereum.pb.h"
#include "Ethereum/ABI/Function.h"
#include "uint256.h"

namespace TW::LiquidStaking {

Proto::LiquidStakingOutput Builder::buildStraderEVM() const {
    Proto::LiquidStakingOutput output;
    if (!mSmartContractAddress) {
        *output.mutable_status() = generateError(Proto::ERROR_SMART_CONTRACT_ADDRESS_NOT_SET, "Strader protocol require the smart contract address to be set");
        return output;
    }
    auto input = Ethereum::Proto::SigningInput();

    input.set_tx_mode(Ethereum::Proto::Enveloped);
    input.set_to_address(*mSmartContractAddress);

    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();

    auto visitFunctor = [&transfer](const TAction& value) {
        // TODO probably need to guess function named based on the coin/blockchain, can refactor later when adding more
        if (auto* stake = std::get_if<Proto::Stake>(&value); stake) {
            auto func = Ethereum::ABI::Function("swapMaticForMaticXViaInstantPool");
            Data payload;
            func.encode(payload);
            transfer.set_data(payload.data(), payload.size());
            Data amountData = store(uint256_t(stake->amount()));
            transfer.set_amount(amountData.data(), amountData.size());
        } else if (auto* unstake = std::get_if<Proto::Unstake>(&value); unstake) {
            auto params = std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(unstake->amount()))};
            auto func = Ethereum::ABI::Function("requestMaticXSwap", params);
            Data payload;
            func.encode(payload);
            transfer.set_data(payload.data(), payload.size());
            Data amountData = store(uint256_t(0));
            transfer.set_amount(amountData.data(), amountData.size());
        }
    };

    std::visit(visitFunctor, this->mAction);


    *output.mutable_ethereum() = input;
    return output;
}

Proto::LiquidStakingOutput Builder::buildStrader() const {
    switch (this->mBlockchain) {
    case Proto::ETHEREUM:
    case Proto::POLYGON:
        return buildStraderEVM();
    default:
        auto output = Proto::LiquidStakingOutput();
        *output.mutable_status() = generateError(Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL, "Only Strader EVM chains is supported for now");
        return output;
    }
}

Proto::LiquidStakingOutput Builder::build() const {
    switch (this->mProtocol) {
    case Proto::Strader:
        return this->buildStrader();
    default:
        return Proto::LiquidStakingOutput();
    }
}


}
