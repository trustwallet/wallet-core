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

/*
Signer::Signer(Proto::TransactionPlan& plan) : plan(plan) {
    tx.set_amount(plan.amount());
    tx.set_from_address(plan.from_address());
    tx.set_to_address(plan.to_address());
    tx.set_remark(plan.remark());
    tx.set_timestamp(plan.timestamp());
    *tx.mutable_inputs() = *plan.mutable_inputs();
    *tx.mutable_outputs() = *plan.mutable_outputs();
}
 */
Signer::Signer(Proto::SigningInput& input) : input(input) {
    Proto::TransactionCoinFrom coinFrom;
    Proto::TransactionCoinTo coinTo;

    coinFrom.set_from_address(input.from_address());
    coinFrom.set_assets_chainid(input.chain_id());
    coinFrom.set_idassets_id(input.idassetsid());
    //need to update with amount + fee
    coinFrom.set_idamount(input.amount());
    coinFrom.set_nonce(input.nonce());
    //default unlocked
    coinFrom.set_locked(0);
    Proto::TransactionCoinFrom* input_item = tx.add_inputs();
    *input_item = coinFrom;

    coinTo.set_to_address(input.to_address());
    coinTo.set_idamount(input.amount());
    coinTo.set_assets_chainid(input.chain_id());
    coinTo.set_idassets_id(input.idassetsid());
    coinTo.set_lock_time(0);
    Proto::TransactionCoinTo* output_item = tx.add_outputs();
    *output_item = coinTo;

    tx.set_remark(input.remark());
    tx.set_type(2);
    tx.set_timestamp(input.timestamp());
    tx.set_tx_data(0xffffffff);
}
Data Signer::sign() const {
    if (input.private_key().empty()) {
        throw std::invalid_argument("Must have private key string");
    }
    /*else if (tx.inputs_size() == 0) {
        throw std::invalid_argument("Not enough input balance to do the transaction");
    } else if (tx.outputs_size() == 0) {
        throw std::invalid_argument("There must be at least one output, something is missed");
    }*/

    auto priv = Address::importHexPrivateKey(input.private_key());

    auto data = Data();

    // Transaction Type
    encode16LE(tx.type(), data);

    // Timestamp
    encode32LE(tx.timestamp(), data);
    
    // txData
    encode32LE(tx.tx_data(), data);

    // CoinData Input
    std::vector<Proto::TransactionCoinFrom> inputs;
    std::copy(tx.inputs().begin(), tx.inputs().end(), std::back_inserter(inputs));
    serializerInput(inputs, data);

    // CoinData Output
    std::vector<Proto::TransactionCoinTo> outputs;
    std::copy(tx.outputs().begin(), tx.outputs().end(), std::back_inserter(outputs));
    serializerOutput(outputs, data);
    
    // Remark
    std::string remark = tx.remark();
    serializerRemark(remark, data);

    // Calc transaction hash
    Data txHash = calcTransactionDigest(data);

    auto transactionSignature = makeTransactionSignature(priv, txHash);

    encodeVarInt(transactionSignature.size(), data);
    std::copy(transactionSignature.begin(), transactionSignature.end(), std::back_inserter(data));

    return data;
}
