// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Transaction.h"
#include "../proto/Cardano.pb.h"
#include "Data.h"

#include <string>

namespace TW::Cardano {

class Signer {
public:
    Proto::SigningInput input;
    TransactionPlan _plan;
    // Sender address, address bytes, derived from private key from input
    Data senderAddress;

    Signer(const Proto::SigningInput& input);

    Proto::SigningOutput sign();
    // Sign using existing plan
    Proto::SigningOutput signWithPlan();
    // Create plan from signing input
    TransactionPlan plan() const;
    // Build output transaction object, using input and plan
    static Common::Proto::SigningError buildTransaction(Transaction& tx, const Proto::SigningInput& input, const TransactionPlan& plan, const Data& senderAddress);
    static Amount estimateFee(const Proto::SigningInput& input, Amount amount, const Data& senderAddress);
    static std::vector<TxInput> selectInputsSimple(const std::vector<TxInput>& inputs, Amount amount);
};

} // namespace TW::Cardano
