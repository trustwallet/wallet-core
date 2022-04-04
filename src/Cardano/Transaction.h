// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "AddressV3.h"

#include "Data.h"
#include "../proto/Cardano.pb.h"
#include "../proto/Common.pb.h"

#include <vector>

namespace TW::Cardano {

typedef uint64_t Amount;

class OutPoint {
public:
    Data txHash;
    uint64_t outputIndex;

    static OutPoint fromProto(const Proto::OutPoint& proto);
};

class TxInput: public OutPoint {
public:
    std::string address;

    /// ADA amount
    Amount amount;

    static TxInput fromProto(const Proto::TxInput& proto);
    Proto::TxInput toProto() const;
};

class TxOutput {
public:
    Data address;
    /// ADA amount
    Amount amount;
};

class TransactionPlan {
public:
    std::vector<TxInput> utxos;
    Amount availableAmount = 0;
    Amount amount = 0;
    Amount fee = 0;
    Amount change = 0;
    Common::Proto::SigningError error = Common::Proto::SigningError::OK;

    static TransactionPlan fromProto(const Proto::TransactionPlan& proto);
    Proto::TransactionPlan toProto() const;
};

class Transaction {
public:
    std::vector<OutPoint> inputs;
    std::vector<TxOutput> outputs;
    Amount fee;
    uint64_t ttl;

    // Encode into CBOR binary format
    Data encode() const;

    // Derive Transaction ID from hashed encoded data
    Data getId() const;
};

} // namespace TW::Cardano
