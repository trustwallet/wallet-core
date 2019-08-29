// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/NEAR.pb.h"
#include <nlohmann/json.hpp>

extern const std::string NEAR_PREFIX_SEND_COIN_MESSAGE;
extern const std::string NEAR_PREFIX_STAKE_MESSAGE;
extern const std::string NEAR_PREFIX_TRANSACTION;
extern const std::string NEAR_PREFIX_UNSTAKE_MESSAGE;
extern const std::string NEAR_PREFIX_WITHDRAW_STAKE_MESSAGE;
extern const std::string NEAR_PREFIX_PUBLIC_KEY;

namespace TW::NEAR {

nlohmann::json signaturePreimageJSON(const Proto::SigningInput& input);
nlohmann::json transactionJSON(const Proto::Transaction& transaction, std::string type_prefix);

} // namespace
