// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../proto/Cardano.pb.h"

namespace TW::Cardano {

class Signer {
public:
    Proto::SigningInput input;
    TransactionPlan _plan;

    Signer(const Proto::SigningInput& input) : input(input) {}

    Proto::SigningOutput sign();
    TransactionPlan plan() const;
};

} // namespace TW::Cardano
