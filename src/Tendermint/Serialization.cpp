// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../Tendermint/Address.h"
#include <TrustWalletCore/TWHRP.h>

using namespace TW;
using namespace TW::Cosmos;

using json = nlohmann::json;

json TW::Cosmos::signatureJSON(const TW::Cosmos::Proto::SigningInput& input) {
    json j;
    j["account_number"] = std::to_string(input.account_number());
    j["chain_id"] = input.chain_id();
    j["memo"] = input.memo();
    j["sequence"] = std::to_string(input.sequence());
    return j;
}
