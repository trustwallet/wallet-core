// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEOPlanner.h>

#include "../NEO/Signer.h"
#include "../proto/NEO.pb.h"

using namespace TW;
using namespace TW::NEO;

TW_NEO_Proto_TransactionPlan TWNEOPlannerPlan(TW_NEO_Proto_SigningInput inputData) {
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
