// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../uint256.h"
#include "ISerializable.h"
#include "Serializable.h"
#include "TransactionType.h"
#include "TransactionAttribute.h"
#include "TransactionOutput.h"
#include "CoinReference.h"
#include "Witness.h"

namespace TW::NEO {

class Transaction : public Serializable {
public:
    TransactionType type;
    byte version;
    std::vector<TransactionAttribute> attributes;
    std::vector<CoinReference> inInputs;
    std::vector<TransactionOutput> outputs;
    std::vector<Witness> witnesses;

    virtual ~Transaction() {}
    int64_t size() const override;
    void deserialize(const Data& data, int initial_pos = 0) override;
    Data serialize() const override;

    bool operator==(const Transaction &other) const;

    virtual int deserializeExclusiveData(const Data& data, int initial_pos = 0) { return initial_pos; }
    virtual Data serializeExclusiveData() const { return Data(); }

    Data getHash() const;
    uint256_t getHashUInt256() const;

    static Transaction * deserializeFrom(const Data& data, int initial_pos = 0);
};

} // namespace TW::NEO
