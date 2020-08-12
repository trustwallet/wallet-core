// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Binance.pb.h"
#include <nlohmann/json.hpp>

namespace TW::Binance {

nlohmann::json signatureJSON(const Proto::SigningInput& input);
nlohmann::json orderJSON(const Proto::SigningInput& input);
nlohmann::json inputsJSON(const Proto::SendOrder& order);
nlohmann::json outputsJSON(const Proto::SendOrder& order);
nlohmann::json tokenJSON(const Proto::SendOrder_Token& token, bool stringAmount = false);
nlohmann::json tokensJSON(const ::google::protobuf::RepeatedPtrField<Proto::SendOrder_Token>& tokens);

} // namespace TW::Binance
