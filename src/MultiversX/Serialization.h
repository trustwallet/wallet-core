// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "Transaction.h"
#include <nlohmann/json.hpp>

namespace TW::MultiversX {

using string = std::string;
using json = nlohmann::json;

string serializeTransaction(const Transaction& transaction);
string serializeSignedTransaction(const Transaction& transaction, string encodedSignature);

} // namespace TW::MultiversX
