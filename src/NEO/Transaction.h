// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "CoinReference.h"
#include "ISerializable.h"
#include "Serializable.h"
#include "TransactionAttribute.h"
#include "TransactionOutput.h"
#include "TransactionType.h"
#include "Witness.h"
#include "../uint256.h"

namespace TW::NEO {

class Transaction : public Serializable {
public:
    TransactionType type;
    byte version;
    std::vector<TransactionAttribute> attributes;
    std::vector<CoinReference> inInputs;
    std::vector<TransactionOutput> outputs;
    std::vector<Witness> witnesses;

    Transaction(TransactionType t = TransactionType::TT_ContractTransaction, byte ver = 0) : type(t), version(ver) {}
    ~Transaction() override = default;

    size_t size() const override;
    void deserialize(const Data& data, size_t initial_pos = 0) override;
    Data serialize() const override;

    bool operator==(const Transaction& other) const;

    virtual size_t deserializeExclusiveData([[maybe_unused]] const Data& data, size_t initial_pos) { return initial_pos; }
    virtual Data serializeExclusiveData() const { return {}; }

    Data getHash() const;
    uint256_t getHashUInt256() const;

    static Transaction* deserializeFrom(const Data& data, size_t initial_pos = 0);
};

} // namespace TW::NEO
