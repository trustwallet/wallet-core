// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "./Protobuf/TronInternal.pb.h"
#include "Data.h"
#include <nlohmann/json.hpp>

namespace TW::Tron {

nlohmann::json transactionJSON(const protocol::Transaction& transaction, const TW::Data& txID, const TW::Data& signature);

}
