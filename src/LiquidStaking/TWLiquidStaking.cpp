// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Data.h"
#include "proto/LiquidStaking.pb.h"
#include "LiquidStaking/LiquidStaking.h"
#include "TrustWalletCore/TWLiquidStaking.h"

using namespace TW;

TWData *_Nonnull TWLiquidStakingBuildRequest(TWData *_Nonnull input) {
    LiquidStaking::Proto::Input inputProto;
    LiquidStaking::Proto::Output outputProto;

    if (!inputProto.ParseFromArray(TWDataBytes(input), static_cast<int>(TWDataSize(input)))) {
        *outputProto.mutable_status() = LiquidStaking::generateError(LiquidStaking::Proto::ERROR_INPUT_PROTO_DESERIALIZATION);
        auto outputData = TW::data(outputProto.SerializeAsString());
        return TWDataCreateWithBytes(outputData.data(), outputData.size());
    }

    outputProto = LiquidStaking::build(inputProto);
    auto outputData = TW::data(outputProto.SerializeAsString());
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}
