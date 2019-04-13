// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "Address.h"
#include "BinaryCoding.h"
#include "../Hash.h"
#include "../PrivateKey.h"

using namespace TW;
using namespace TW::NULS;

Signer::Signer(Proto::TransactionPlan& plan) : plan(plan) {
    tx.set_amount(plan.amount());
    tx.set_from_address(plan.from_address());
    tx.set_to_address(plan.to_address());
    tx.set_remark(plan.remark());
    tx.set_timestamp(plan.timestamp());
    *tx.mutable_inputs() = *plan.mutable_inputs();
    *tx.mutable_outputs() = *plan.mutable_outputs();
}

std::vector<uint8_t> Signer::sign() const {
    if (plan.private_key().empty()) {
        throw std::invalid_argument("Must have private key string");
    } else if (tx.inputs_size() == 0) {
        throw std::invalid_argument("Not enough input balance to do the transaction");
    } else if (tx.outputs_size() == 0) {
        throw std::invalid_argument("There must be at least one output, something is missed");
    }
    auto priv = Address::importHexPrivateKey(plan.private_key());

    auto data = Data();
    // Transaction Type
    encode16LE(2, data);
    // Timestamp
    encode48LE(tx.timestamp(), data);
    // Remark
    std::string remark = tx.remark();
    serializerRemark(remark, data);
    // txData
    encode32LE(0xffffffff, data);
    // CoinData Input
    std::vector<Proto::TransactionInput> inputs;
    std::copy(tx.inputs().begin(), tx.inputs().end(), std::back_inserter(inputs));
    serializerInput(inputs, data);
    // CoinData Output
    std::vector<Proto::TransactionOutput> outputs;
    std::copy(tx.outputs().begin(), tx.outputs().end(), std::back_inserter(outputs));
    serializerOutput(outputs, data);

    // Calc transaction hash
    Data txHash = calcTransactionDigest(data);

    auto transactionSignature = makeTransactionSignature(priv, txHash);

    encodeVarInt(transactionSignature.size(), data);
    std::copy(transactionSignature.begin(), transactionSignature.end(), std::back_inserter(data));

    return data;
}
