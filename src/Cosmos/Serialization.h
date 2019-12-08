// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Cosmos.pb.h"
#include <nlohmann/json.hpp>

using string = std::string;

extern const string TYPE_PREFIX_MSG_SEND;
extern const string TYPE_PREFIX_MSG_DELEGATE;
extern const string TYPE_PREFIX_MSG_UNDELEGATE;
extern const string TYPE_PREFIX_MSG_REDELEGATE;
extern const string TYPE_PREFIX_MSG_WITHDRAW_REWARD;
extern const string TYPE_PREFIX_MSG_WITHDRAW_REWARDS_ALL;
extern const string TYPE_PREFIX_PUBLIC_KEY;

namespace TW::Cosmos {

nlohmann::json signaturePreimageJSON(const Proto::SigningInput& input);
nlohmann::json transactionJSON(const Proto::Transaction& transaction, const string& type_prefix);

} // namespace
