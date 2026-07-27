// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Tron/Protobuf/TronInternal.pb.h"
#include "Result.h"
#include <nlohmann/json.hpp>
#include <string>

namespace TW::Tron {

constexpr size_t MAX_JSON_SIZE = 1024 * 1024; // 1 MB

/// Deserializes a Tron node JSON transaction (as produced by `transactionJSON`) back into a
/// `protocol::Transaction`. All `bytes` fields are expected to be hex-encoded strings, matching
/// the format used by Tron nodes and the wallet-core serialization layer.
///
/// Returns a failure Result with an error message if the JSON is malformed or contains an
/// unsupported contract type.
Result<protocol::Transaction> transactionFromJSON(const std::string& json);

/// Deserializes a Tron node JSON transaction (as produced by `transactionJSON`) back into a
/// `protocol::Transaction`. All `bytes` fields are expected to be hex-encoded strings, matching
/// the format used by Tron nodes and the wallet-core serialization layer.
///
/// Returns a failure Result with an error message if the JSON is malformed or contains an
/// unsupported contract type.
Result<protocol::Transaction> transactionFromJSONObject(const nlohmann::json& json);

} // namespace TW::Tron
