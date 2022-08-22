// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PublicKey.h"
#include "../proto/Cosmos.pb.h"
#include <nlohmann/json.hpp>

extern const std::string TYPE_PREFIX_MSG_SEND;
extern const std::string TYPE_PREFIX_MSG_TRANSFER;
extern const std::string TYPE_PREFIX_MSG_DELEGATE;
extern const std::string TYPE_PREFIX_MSG_UNDELEGATE;
extern const std::string TYPE_PREFIX_MSG_REDELEGATE;
extern const std::string TYPE_PREFIX_MSG_WITHDRAW_REWARD;
extern const std::string TYPE_PREFIX_PUBLIC_KEY;

namespace TW::Cosmos::Json {

using string = std::string;
using json = nlohmann::json;

json signaturePreimageJSON(const Proto::SigningInput& input);

json transactionJSON(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature);

std::string buildJsonTxRaw(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature);
json transactionJSON(const Proto::SigningInput& input, const Data& signature);

} // namespace TW::Cosmos::json
