// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CellInput.h"
#include "Serialization.h"

namespace TW::Nervos {

void CellInput::encode(Data& data) const {
    encode64LE(since, data);
    previousOutput.encode(data);
}

nlohmann::json CellInput::json() const {
    return nlohmann::json{{"previous_output", previousOutput.json()},
                          {"since", Serialization::numberToHex(since)}};
}

} // namespace TW::Nervos
