// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "Swap.h"
#include "proto/THORChainSwap.pb.h"
#include <TrustWalletCore/TWTHORChainSwap.h>

using namespace TW;

TWData* _Nonnull TWTHORChainSwapBuildSwap(TWData* _Nonnull input) {
    THORChainSwap::Proto::SwapInput inputProto;
    THORChainSwap::Proto::SwapOutput outputProto;
    if (!inputProto.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)))) {
        // error
        outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
        outputProto.mutable_error()->set_message("Could not deserialize input proto");
        auto outputData = TW::data(outputProto.SerializeAsString());
        return TWDataCreateWithBytes(outputData.data(), outputData.size());
    }

    const auto fromChain = inputProto.from_asset().chain();
    const auto toChain = inputProto.to_asset().chain();
    auto builder = THORChainSwap::SwapBuilder::builder();
    builder
        .from(inputProto.from_asset())
        .to(inputProto.to_asset())
        .fromAddress(inputProto.from_address())
        .toAddress(inputProto.to_address())
        .vault(inputProto.vault_address())
        .router(inputProto.router_address())
        .fromAmount(inputProto.from_amount())
        .toAmountLimit(inputProto.to_amount_limit())
        .affFeeAddress(inputProto.affiliate_fee_address())
        .affFeeRate(inputProto.affiliate_fee_rate_bp())
        .extraMemo(inputProto.extra_memo())
        .expirationPolicy(inputProto.expiration_time());
    if (inputProto.has_stream_params()) {
        const auto& streamParams = inputProto.stream_params();
        builder
            .streamInterval(streamParams.interval())
            .streamQuantity(streamParams.quantity());
    }

    auto&& [txInput, errorCode, error] = builder.build();
    outputProto.set_from_chain(fromChain);
    outputProto.set_to_chain(toChain);
    if (errorCode != 0) {
        // error
        outputProto.mutable_error()->set_code(static_cast<THORChainSwap::Proto::ErrorCode>(errorCode));
        outputProto.mutable_error()->set_message(error);
    } else {
        // no error
        outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::OK);
        outputProto.mutable_error()->set_message("");

        switch (fromChain) {
        case THORChainSwap::Proto::BTC:
        case THORChainSwap::Proto::DOGE:
        case THORChainSwap::Proto::BCH:
        case THORChainSwap::Proto::LTC: {
            Bitcoin::Proto::SigningInput btcInput;
            if (!btcInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize BTC input");
            } else {
                *outputProto.mutable_bitcoin() = btcInput;
            }
        } break;

        case THORChainSwap::Proto::ETH:
        case THORChainSwap::Proto::BSC:
        case THORChainSwap::Proto::AVAX: {
            Ethereum::Proto::SigningInput ethInput;
            if (!ethInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize ETH input");
            } else {
                *outputProto.mutable_ethereum() = ethInput;
            }
        } break;

        case THORChainSwap::Proto::BNB: {
            Binance::Proto::SigningInput bnbInput;
            if (!bnbInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize BNB input");
            } else {
                *outputProto.mutable_binance() = bnbInput;
            }
        } break;

        case THORChainSwap::Proto::THOR:
        case THORChainSwap::Proto::ATOM: {
            Cosmos::Proto::SigningInput cosmosInput;
            if (!cosmosInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize ATOM input");
            } else {
                *outputProto.mutable_cosmos() = cosmosInput;
            }
        } break;

        default:
            outputProto.mutable_error()->set_code(THORChainSwap::Proto::ErrorCode::Error_Unsupported_from_chain);
            outputProto.mutable_error()->set_message(std::string("Unsupported from chain ") + std::to_string(fromChain));
            break;
        }
    }

    // serialize output
    auto outputData = TW::data(outputProto.SerializeAsString());
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}
