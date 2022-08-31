// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTHORChainSwap.h>
#include "Swap.h"
#include "proto/THORChainSwap.pb.h"
#include "Data.h"

TWData* _Nonnull TWTHORChainSwapBuildSwap(TWData* _Nonnull input) {
    TW::THORChainSwap::Proto::SwapInput inputProto;
    TW::THORChainSwap::Proto::SwapOutput outputProto;
    if (!inputProto.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)))) {
        // error
        outputProto.mutable_error()->set_code(TW::THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
        outputProto.mutable_error()->set_message("Could not deserialize input proto");
        auto outputData = TW::data(outputProto.SerializeAsString());
        return TWDataCreateWithBytes(outputData.data(), outputData.size());
    }

    const auto fromChain = inputProto.from_chain();
    const auto toChain = inputProto.to_asset().chain();
    auto res = TW::THORChainSwap::Swap::build(
        static_cast<TW::THORChainSwap::Chain>(static_cast<int>(fromChain)),
        static_cast<TW::THORChainSwap::Chain>(static_cast<int>(toChain)),
        inputProto.from_address(),
        inputProto.to_asset().symbol(),
        inputProto.to_asset().token_id(),
        inputProto.to_address(),
        inputProto.vault_address(),
        inputProto.router_address(),
        inputProto.from_amount(),
        inputProto.to_amount_limit());

    outputProto.set_from_chain(fromChain);
    outputProto.set_to_chain(toChain);
    if (std::get<1>(res) != 0) {
        // error
        outputProto.mutable_error()->set_code(static_cast<TW::THORChainSwap::Proto::ErrorCode>(std::get<1>(res)));
        outputProto.mutable_error()->set_message(std::get<2>(res));
    } else {
        // no error
        outputProto.mutable_error()->set_code(TW::THORChainSwap::Proto::ErrorCode::OK);
        outputProto.mutable_error()->set_message("");

        const TW::Data& txInput = std::get<0>(res);
        switch (fromChain) {
        case TW::THORChainSwap::Proto::BTC: {
            TW::Bitcoin::Proto::SigningInput btcInput;
            if (!btcInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(TW::THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize BTC input");
            } else {
                *outputProto.mutable_bitcoin() = btcInput;
            }
        } break;

        case TW::THORChainSwap::Proto::ETH: {
            TW::Ethereum::Proto::SigningInput ethInput;
            if (!ethInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(TW::THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize ETH input");
            } else {
                *outputProto.mutable_ethereum() = ethInput;
            }
        } break;

        case TW::THORChainSwap::Proto::BNB: {
            TW::Binance::Proto::SigningInput bnbInput;
            if (!bnbInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                outputProto.mutable_error()->set_code(TW::THORChainSwap::Proto::ErrorCode::Error_Input_proto_deserialization);
                outputProto.mutable_error()->set_message("Could not deserialize BNB input");
            } else {
                *outputProto.mutable_binance() = bnbInput;
            }
        } break;

        default:
            outputProto.mutable_error()->set_code(TW::THORChainSwap::Proto::ErrorCode::Error_Unsupported_from_chain);
            outputProto.mutable_error()->set_message(std::string("Unsupported from chain ") + std::to_string(fromChain));
            break;
        }
    }

    // serialize output
    auto outputData = TW::data(outputProto.SerializeAsString());
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}
