// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
