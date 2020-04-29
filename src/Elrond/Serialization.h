// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Elrond.pb.h"
#include "Data.h"
#include <nlohmann/json.hpp>

using string = std::string;
using json = nlohmann::json;

namespace TW::Elrond {

string serializeTransaction(const Proto::TransactionMessage& message);
string serializeSignedTransaction(const Proto::TransactionMessage& message, string encodedSignature);

} // namespace
