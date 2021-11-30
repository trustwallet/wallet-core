// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Swap.h"

#include <TrustWalletCore/TWCoinType.h>
#include "Coin.h"
#include "proto/THORChainSwap.pb.h"
// BTC
#include "Bitcoin/SigHashType.h"
#include "../proto/Bitcoin.pb.h"
// ETH
#include "Ethereum/Address.h"
#include "Ethereum/ABI/Function.h"
#include "Ethereum/ABI/ParamBase.h"
#include "Ethereum/ABI/ParamAddress.h"
#include "uint256.h"
#include "../proto/Ethereum.pb.h"
// BNB
#include "Binance/Address.h"
#include "../proto/Binance.pb.h"

#include <cstdlib>

/*
 * References:
 *  https://gitlab.com/thorchain/asgardex-common/asgardex-util
 */

namespace TW::THORChainSwap {

TWCoinType chainCoinType(Chain chain) {
    switch (chain) {
        case Chain::ETH: return TWCoinTypeEthereum;
        case Chain::BNB: return TWCoinTypeBinance;
        case Chain::BTC: return TWCoinTypeBitcoin;
        case Chain::THOR:
        default:
            return TWCoinTypeTHORChain;
    }
}

std::string chainName(Chain chain) {
    switch (chain) {
        case Chain::ETH: return "ETH";
        case Chain::BNB: return "BNB";
        case Chain::BTC: return "BTC";
        case Chain::THOR:
        default:
            return "THOR";
    }
}

std::string Swap::buildMemo(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& toAddress, uint64_t limit) {
    std::string prefix = "SWAP";
    if (toChain == Chain::ETH) {
        prefix = "=";
    }
    const auto toCoinToken = (!toTokenId.empty() && toTokenId != "0x0000000000000000000000000000000000000000") ? toTokenId : toSymbol;
    return prefix + ":" + chainName(toChain) + "." + toCoinToken + ":" + toAddress + ":" + std::to_string(limit);
}

bool validateAddress(Chain chain, const std::string& address) {
    return TW::validateAddress(chainCoinType(chain), address);
}

std::tuple<Data, int, std::string> Swap::build(
    Chain fromChain,
    Chain toChain,
    const std::string& fromAddress,
    const std::string& toSymbol,
    const std::string& toTokenId,
    const std::string& toAddress,
    const std::string& vaultAddress,
    const std::string& routerAddress,
    const std::string& fromAmount,
    const std::string& toAmountLimit
)  {
    if (!validateAddress(fromChain, fromAddress)) {
        return std::make_tuple<Data, int, std::string>({}, static_cast<int>(Proto::ErrorCode::Error_Invalid_from_address), "Invalid from address");
    }
    if (!validateAddress(toChain, toAddress)) {
        return std::make_tuple<Data, int, std::string>({}, static_cast<int>(Proto::ErrorCode::Error_Invalid_to_address), "Invalid to address");
    }

    uint64_t fromAmountNum = std::atoll(fromAmount.c_str());
    uint64_t toAmountLimitNum = std::atoll(toAmountLimit.c_str());
    const auto memo = buildMemo(toChain, toSymbol, toTokenId, toAddress, toAmountLimitNum);

    switch (fromChain) {
        case Chain::BTC: {
            Data out;
            auto res = buildBitcoin(toChain, toSymbol, toTokenId, fromAddress, toAddress, vaultAddress, fromAmountNum, memo, out);
            return std::make_tuple<Data, int, std::string>(std::move(out), std::move(std::get<0>(res)), std::move(std::get<1>(res)));
        }

        case Chain::ETH: {
            Data out;
            auto res = buildEthereum(toChain, toSymbol, toTokenId, fromAddress, toAddress, vaultAddress, routerAddress, fromAmountNum, memo, out);
            return std::make_tuple<Data, int, std::string>(std::move(out), std::move(std::get<0>(res)), std::move(std::get<1>(res)));
        }

        case Chain::BNB: {
            Data out;
            auto res = buildBinance(toChain, toSymbol, toTokenId, fromAddress, toAddress, vaultAddress, fromAmountNum, memo, out);
            return std::make_tuple<Data, int, std::string>(std::move(out), std::move(std::get<0>(res)), std::move(std::get<1>(res)));
        }

        case Chain::THOR:
        default:
            return std::make_tuple<Data, int, std::string>({}, static_cast<int>(Proto::ErrorCode::Error_Unsupported_from_chain), "Unsupported from chain: " + std::to_string(toChain));
    }
}

std::pair<int, std::string> Swap::buildBitcoin(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& fromAddress, const std::string& toAddress, const std::string& vaultAddress, uint64_t amount, const std::string& memo, Data& out) {
    auto input = Bitcoin::Proto::SigningInput();

    // Following fields must be set afterwards, before signing ...
    input.set_hash_type(TWBitcoinSigHashTypeAll);
    input.set_byte_fee(1);
    input.set_use_max_amount(false);
    // private_key[]
    // utxo[]
    // scripts[]
    // ... end

    input.set_amount(amount);
    input.set_to_address(vaultAddress);
    input.set_change_address(fromAddress);
    input.set_coin_type(TWCoinTypeBitcoin);
    input.set_output_op_return(memo);

    auto serialized = input.SerializeAsString();
    out.insert(out.end(), serialized.begin(), serialized.end());
    return std::make_pair<int, std::string>(0, "");
}

Data ethAddressStringToData(const std::string& asString) {
    Data asData(20);
    if (asString.empty() || !Ethereum::Address::isValid(asString)) {
        return asData;
    }
    auto address = Ethereum::Address(asString);
    std::copy(address.bytes.begin(), address.bytes.end(), asData.data());
    return asData;
}

std::pair<int, std::string> Swap::buildEthereum(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& fromAddress, const std::string& toAddress, const std::string& vaultAddress, const std::string& routerAddress, uint64_t amount, const std::string& memo, Data& out) {
    auto input = Ethereum::Proto::SigningInput();

    // some sanity check / address conversion
    Data vaultAddressBin = ethAddressStringToData(vaultAddress);
    if (!Ethereum::Address::isValid(vaultAddress) || vaultAddressBin.size() != Ethereum::Address::size) {
        return std::make_pair<int, std::string>(static_cast<int>(Proto::ErrorCode::Error_Invalid_vault_address), "Invalid vault address: " + vaultAddress);
    }
    if (!Ethereum::Address::isValid(routerAddress)) {
        return std::make_pair<int, std::string>(static_cast<int>(Proto::ErrorCode::Error_Invalid_router_address), "Invalid router address: " + routerAddress);
    }
    Data toAssetAddressBin = ethAddressStringToData(toTokenId);

    // Following fields must be set afterwards, before signing ...
    const auto chainId = store(uint256_t(0));
    input.set_chain_id(chainId.data(), chainId.size());
    const auto nonce = store(uint256_t(0));
    input.set_nonce(nonce.data(), nonce.size());
    const auto gasPrice = store(uint256_t(0));
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    const auto gasLimit = store(uint256_t(0));
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_private_key("");
    // ... end

    input.set_to_address(routerAddress);
    auto& transfer = *input.mutable_transaction()->mutable_contract_generic();
    auto func = Ethereum::ABI::Function("deposit", std::vector<std::shared_ptr<Ethereum::ABI::ParamBase>>{
        std::make_shared<Ethereum::ABI::ParamAddress>(vaultAddressBin),
        std::make_shared<Ethereum::ABI::ParamAddress>(toAssetAddressBin),
        std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(amount)),
        std::make_shared<Ethereum::ABI::ParamString>(memo)
    });
    Data payload;
    func.encode(payload);
    transfer.set_data(payload.data(), payload.size());
    Data amountData = store(uint256_t(amount));
    transfer.set_amount(amountData.data(), amountData.size());

    auto serialized = input.SerializeAsString();
    out.insert(out.end(), serialized.begin(), serialized.end());
    return std::make_pair<int, std::string>(0, "");
}

std::pair<int, std::string> Swap::buildBinance(Chain toChain, const std::string& toSymbol, const std::string& toTokenId, const std::string& fromAddress, const std::string& toAddress, const std::string& vaultAddress, uint64_t amount, const std::string& memo, Data& out) {
    auto input = Binance::Proto::SigningInput();

    // Following fields must be set afterwards, before signing ...
    input.set_chain_id("");
    input.set_account_number(0);
    input.set_sequence(0);
    input.set_source(0);
    input.set_private_key("");
    // ... end

    input.set_memo(memo);

    auto& order = *input.mutable_send_order();

    auto token = Binance::Proto::SendOrder::Token();
    token.set_denom("BNB");
    token.set_amount(amount);
    {
        Binance::Address fromAddressBin;
        Binance::Address::decode(fromAddress, fromAddressBin);
        auto input = order.add_inputs();
        input->set_address(fromAddressBin.getKeyHash().data(), fromAddressBin.getKeyHash().size());
        *input->add_coins() = token;
    }
    {
        Binance::Address vaultAddressBin;
        Binance::Address::decode(vaultAddress, vaultAddressBin);
        auto output = order.add_outputs();
        output->set_address(vaultAddressBin.getKeyHash().data(), vaultAddressBin.getKeyHash().size());
        *output->add_coins() = token;
    }

    auto serialized = input.SerializeAsString();
    out.insert(out.end(), serialized.begin(), serialized.end());
    return std::make_pair<int, std::string>(0, "");
}

} // namespace TW
