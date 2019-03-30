// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Binance.pb.h"
#include <nlohmann/json.hpp>

namespace TW::Binance {

nlohmann::json signatureJSON(const Binance::Proto::SigningInput& input);
nlohmann::json orderJSON(const Binance::Proto::SigningInput& input);
nlohmann::json inputsJSON(const Binance::Proto::SendOrder& order, bool testNet);
nlohmann::json outputsJSON(const Binance::Proto::SendOrder& order, bool testNet);
nlohmann::json tokensJSON(const ::google::protobuf::RepeatedPtrField<Binance::Proto::SendOrder_Token>& tokens);

} // namespace TW::Binance
