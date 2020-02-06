// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEOSigner.h>

#include "../NEO/Signer.h"
#include "../proto/NEO.pb.h"

using namespace TW;
using namespace TW::NEO;


TW_NEO_Proto_TransactionPlan TWNEOSignerPlanTransaction(TW_NEO_Proto_SigningInput inputData) {
    Proto::SigningInput input;
    Proto::TransactionPlan plan;
    if (!input.ParseFromArray(TWDataBytes(inputData), static_cast<int>(TWDataSize(inputData)))) {
        // failed to parse input, return empty output with error
        plan.set_error("Error: could not parse input");
    } else {
        try {
            plan = Signer::planTransaction(std::move(input));
        }
        catch (char const *error) {
            plan.set_error(error);
        }
    }
    auto serialized = plan.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}

TW_NEO_Proto_SigningOutput TWNEOSignerSign(TW_NEO_Proto_SigningInput inputData, TW_NEO_Proto_TransactionPlan planData) {
    Proto::SigningInput input;
    Proto::TransactionPlan plan;
    Proto::SigningOutput output;
    if (!input.ParseFromArray(TWDataBytes(inputData), static_cast<int>(TWDataSize(inputData))) ||
        !plan.ParseFromArray(TWDataBytes(planData), static_cast<int>(TWDataSize(planData)))) {
        // failed to parse input/plan, return empty output with error
        output.set_error("Error: could not parse input/plan");
    } else {
        try {
            output = Signer::sign(std::move(input), std::move(plan));
        }
        catch (...) {
        }
    }
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}
