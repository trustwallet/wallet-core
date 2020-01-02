// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCardanoSigner.h>

#include "../Cardano/Signer.h"
#include "../proto/Cardano.pb.h"

using namespace TW;
using namespace TW::Cardano;

TW_Cardano_Proto_TransactionPlan TWCardanoSignerPlanTransaction(TW_Cardano_Proto_SigningInput inputData) {
    Proto::SigningInput input;
    Proto::TransactionPlan plan;
    if (!input.ParseFromArray(TWDataBytes(inputData), static_cast<int>(TWDataSize(inputData)))) {
        // failed to parse input, return empty output with error
        plan.set_error("Error: could not parse input");
    } else {
        plan = Signer::planTransaction(std::move(input));
    }
    auto serialized = plan.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}

TW_Cardano_Proto_SigningOutput TWCardanoSignerSign(TW_Cardano_Proto_SigningInput inputData, TW_Cardano_Proto_TransactionPlan planData) {
    Proto::SigningInput input;
    Proto::TransactionPlan plan;
    Proto::SigningOutput output;
    if (!input.ParseFromArray(TWDataBytes(inputData), static_cast<int>(TWDataSize(inputData))) ||
        !plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)))) {
        // failed to parse input/plan, return empty output with error
        output.set_error("Error: could not parse input/plan");
    } else {
        output = Signer::sign(std::move(input), std::move(plan));
    }
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}
