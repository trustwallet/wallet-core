// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "../proto/Cardano.pb.h"
#include "../proto/Common.pb.h"

#include <vector>

namespace TW::Cardano {

typedef uint64_t Amount; // TODO proper type

class Utxo {
public:
    Data txHash;
    std::string address;
    Amount amount; // ADA amount 
    uint64_t outputIndex;

    static Utxo fromProto(const Proto::Utxo& proto);
};

class TransactionPlan {
public:
    Amount amount = 0;
    Amount availableAmount = 0;
    Amount fee = 0;
    Amount change = 0;
    std::vector<Utxo> utxos;
    Common::Proto::SigningError error = Common::Proto::SigningError::OK;
};

} // namespace TW::Cardano
