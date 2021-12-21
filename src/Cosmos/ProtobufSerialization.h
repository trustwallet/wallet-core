// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Cosmos.pb.h"
#include "PublicKey.h"
#include "Data.h"

#include <string>

using string = std::string;

extern const string TYPE_PREFIX_MSG_SEND;
extern const string TYPE_PREFIX_MSG_DELEGATE;
extern const string TYPE_PREFIX_MSG_UNDELEGATE;
extern const string TYPE_PREFIX_MSG_REDELEGATE;
extern const string TYPE_PREFIX_MSG_WITHDRAW_REWARD;
extern const string TYPE_PREFIX_PUBLIC_KEY;

namespace TW::Cosmos {

std::string buildProtoTxBody(const Proto::SigningInput& input);

std::string buildAuthInfo(const Proto::SigningInput& input);

std::string signaturePreimageProto(const Proto::SigningInput& input, const PublicKey& publicKey);

std::string buildProtoTxRaw(const Proto::SigningInput& input, const PublicKey& publicKey, const Data& signature);

} // namespace
