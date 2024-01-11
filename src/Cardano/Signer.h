// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Transaction.h"
#include "../proto/Cardano.pb.h"
#include "Data.h"

#include <string>
#include <map>
#include <utility>

namespace TW::Cardano {

class Signer {
public:
    /// Signs a Proto::SigningInput transaction
    static Proto::SigningOutput sign(const Proto::SigningInput& input) noexcept {
        Signer signer = Signer(input);
        return signer.sign();
    }

public:
    Proto::SigningInput input;
    TransactionPlan _plan;

    explicit Signer(Proto::SigningInput input): input(std::move(input)) {}

    Proto::SigningOutput sign();
    // Sign using existing plan
    Proto::SigningOutput signWithPlan() const;
    // Create plan from signing input
    TransactionPlan doPlan() const;
    /// Returns a transaction plan (utxo selection, fee estimation)
    static Proto::TransactionPlan plan(const Proto::SigningInput& input) noexcept {
        const auto signer = Signer(input);
        const auto plan = signer.doPlan();
        return plan.toProto();
    }
    // Build encoded transaction
    static Common::Proto::SigningError encodeTransaction(Data& encoded, Data& txId, const Proto::SigningInput& input, const TransactionPlan& plan, bool sizeEstimationOnly = false);
    static Data encodeTransactionWithSig(const Proto::SigningInput &input, const PublicKey &publicKey, const Data &signature);
    // Build aux transaction object, using input and plan
    static Common::Proto::SigningError buildTransactionAux(Transaction& tx, const Proto::SigningInput& input, const TransactionPlan& plan);
    static Common::Proto::SigningError buildTx(Transaction& tx, const Proto::SigningInput& input);
    static Amount estimateFee(const Proto::SigningInput& input, Amount amount, const TokenBundle& requestedTokens, const std::vector<TxInput>& selectedInputs, const std::vector<TxOutput>& extraOutputs);
    static std::vector<TxInput> selectInputsWithTokens(const std::vector<TxInput>& inputs, Amount amount, const TokenBundle& requestedTokens);
    // Build list of public keys + signature
    static Common::Proto::SigningError assembleSignatures(std::vector<std::pair<Data, Data>>& signatures, const Proto::SigningInput& input, const TransactionPlan& plan, const Data& txId, bool sizeEstimationOnly = false);
};

} // namespace TW::Cardano
