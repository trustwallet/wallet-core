// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"
#include "../Data.h"
#include "../proto/Waves.pb.h"
#include <nlohmann/json.hpp>

namespace TW::Waves {

enum class TransactionType { transfer = 4 };
enum class TransactionVersion { V1 = 1, V2 = 2 };

class Transaction {
    /// We only support Transfer V2 transaction.
    /// See
    /// https://docs.wavesplatform.com/en/blockchain/waves-protocol/data-structures.html#section-8555a9aaf83a8d01f18a2c38d19484fe
  public:
    static const std::string WAVES;

    int64_t amount;
    std::string asset;
    int64_t fee;
    std::string fee_asset;
    Address to;
    Data attachment;
    int64_t timestamp;
    Data pub_key;

    Transaction(int64_t amount, std::string asset, int64_t fee, std::string fee_asset,
                Address to, Data attachment, int64_t timestamp, Data pub_key)
        : amount(std::move(amount))
        , fee(std::move(fee))
        , to(std::move(to))
        , attachment(std::move(attachment))
        , timestamp(std::move(timestamp))
        , pub_key(std::move(pub_key)) {
        if (asset.empty()) {
            asset = WAVES;
        }
        this->asset = asset;
        if (fee_asset.empty()) {
            fee_asset = WAVES;
        }
        this->fee_asset = fee_asset;
    }

  public:
    Data serializeToSign() const;
    nlohmann::json buildJson(Data signature) const;
};

} // namespace TW::Waves
