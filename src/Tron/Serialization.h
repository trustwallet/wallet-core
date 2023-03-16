// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "./Protobuf/TronInternal.pb.h"
#include "Data.h"
#include <nlohmann/json.hpp>

namespace TW::Tron {

nlohmann::json transactionJSON(const protocol::Transaction& transaction, const TW::Data& txID, const TW::Data& signature);

}
