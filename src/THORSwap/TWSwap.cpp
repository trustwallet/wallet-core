// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTHORSwap.h>
#include "Swap.h"
#include "proto/THORSwap.pb.h"
#include "Data.h"

using namespace TW::THORSwap;
using namespace TW;

TWData *_Nonnull TWTHORSwapSwap(TWData *_Nonnull input) {
    Proto::SwapInput inputProto;
    Proto::SwapOutput outputProto;
    if (!inputProto.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)))) {
        // error
        outputProto.set_error("Could not deserialize input proto");
        auto outputData = data(outputProto.SerializeAsString());
        return TWDataCreateWithBytes(outputData.data(), outputData.size());
    }

    const auto fromChain = inputProto.from_chain();
    const auto toChain = inputProto.to_asset().chain();
    auto res = Swap::build(
        static_cast<Chain>(static_cast<int>(fromChain)),
        static_cast<Chain>(static_cast<int>(toChain)),
        inputProto.from_address(),
        inputProto.to_asset().symbol(),
        inputProto.to_asset().token_id(),
        inputProto.to_address(),
        inputProto.vault_address(),
        inputProto.router_address(),
        inputProto.from_amount(),
        inputProto.to_amount_limit()
    );

    outputProto.set_from_chain(fromChain);
    outputProto.set_to_chain(toChain);
    if (std::get<1>(res).length() > 0) {
        // error
        outputProto.set_error(std::get<1>(res));
    } else {
        // no error
        outputProto.set_error("");

        const Data& txInput = std::get<0>(res);
        switch (fromChain) {
            case Proto::BTC:
                {
                    Bitcoin::Proto::SigningInput btcInput;
                    if (!btcInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                        outputProto.set_error("Could not deserialize BTC input");
                    } else {
                        *outputProto.mutable_bitcoin() = btcInput;
                    }
                }
                break;

            case Proto::ETH:
                {
                    Ethereum::Proto::SigningInput ethInput;
                    if (!ethInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                        outputProto.set_error("Could not deserialize ETH input");
                    } else {
                        *outputProto.mutable_ethereum() = ethInput;
                    }
                }
                break;

            case Proto::BNB:
                {
                    Binance::Proto::SigningInput bnbInput;
                    if (!bnbInput.ParseFromArray(txInput.data(), static_cast<int>(txInput.size()))) {
                        outputProto.set_error("Could not deserialize BNB input");
                    } else {
                        *outputProto.mutable_binance() = bnbInput;
                    }
                }
                break;

            default:
                outputProto.set_error(std::string("Unsupported fromChain ") + std::to_string(fromChain));
                break;
        }
    }

    // serialize output
    auto outputData = data(outputProto.SerializeAsString());
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}
