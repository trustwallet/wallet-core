// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
