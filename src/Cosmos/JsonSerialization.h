// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Cosmos.pb.h"
#include "Data.h"
#include <nlohmann/json.hpp>

using string = std::string;
using json = nlohmann::json;

extern const string TYPE_PREFIX_MSG_SEND;
extern const string TYPE_PREFIX_MSG_TRANSFER;
extern const string TYPE_PREFIX_MSG_DELEGATE;
extern const string TYPE_PREFIX_MSG_UNDELEGATE;
extern const string TYPE_PREFIX_MSG_REDELEGATE;
extern const string TYPE_PREFIX_MSG_WITHDRAW_REWARD;
extern const string TYPE_PREFIX_PUBLIC_KEY;

namespace TW::Cosmos {

json signaturePreimageJSON(const Proto::SigningInput& input);
json transactionJSON(const Proto::SigningInput& input, const Data& signature);

} // namespace
