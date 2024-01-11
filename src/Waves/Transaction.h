// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Address.h"
#include "Data.h"
#include "../proto/Waves.pb.h"
#include <nlohmann/json.hpp>

namespace TW::Waves {

enum class TransactionType { transfer = 4, lease = 8, cancelLease = 9 };
enum class TransactionVersion { V1 = 1, V2 = 2 };

class Transaction {
    /// We only support Transfer V2 transaction.
    /// See
    /// https://docs.wavesplatform.com/en/blockchain/waves-protocol/data-structures.html#section-8555a9aaf83a8d01f18a2c38d19484fe
  public:
    static const std::string WAVES;
    const Proto::SigningInput& input;
    const Data pub_key;
    
    Transaction(const Proto::SigningInput& input, const Data pub_key) : input(input), pub_key(std::move(pub_key)) {};

  public:
    Data serializeToSign() const;
    nlohmann::json buildJson(const Data& signature) const;
};

} // namespace TW::Waves
