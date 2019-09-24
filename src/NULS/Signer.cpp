// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "Address.h"
#include "BinaryCoding.h"
#include "TransactionBuilder.h"
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
    coinFrom.set_idassets_id(input.idassets_id());
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
    coinTo.set_idassets_id(input.idassets_id());
    coinTo.set_lock_time(0);
    Proto::TransactionCoinTo* output_item = tx.add_outputs();
    *output_item = coinTo;

    tx.set_remark(input.remark());
    tx.set_type(2);
    tx.set_timestamp(input.timestamp());
    tx.set_tx_data(0);
}

Data Signer::sign() const {

    if (input.private_key().empty()) {
        throw std::invalid_argument("Must have private key string");
    }

    uint32_t txSize = TransactionBuilder::calculatorTransactionSize(1, 1, static_cast<uint32_t>(tx.remark().size()));
    uint64_t fee = TransactionBuilder::calculatorTransactionFee(txSize);
    uint256_t txAmount = load(input.amount());
    uint256_t balance = load(input.balance());
    uint256_t fromAmount = txAmount + fee;
    if (fromAmount > balance) {
        throw std::invalid_argument("User account balance not sufficient");
    }
    
    Proto::TransactionCoinFrom& coinFrom = (Proto::TransactionCoinFrom&)tx.inputs(0);
    Data amount = store(fromAmount);
    std::string amountStr;
    amountStr.insert(amountStr.begin(), amount.begin(), amount.end());
    coinFrom.set_idamount(amountStr);

    auto data = Data();
    // Transaction Type
    encode16LE(tx.type(), data);
    // Timestamp
    encode32LE(tx.timestamp(), data);
     // Remark
    std::string remark = tx.remark();
    serializerRemark(remark, data);
    // txData
    encodeVarInt(0, data);

    //coinFrom and coinTo size
    encodeVarInt(TRANSACTION_INPUT_SIZE + TRANSACTION_OUTPUT_SIZE, data);

    // CoinData Input
    std::vector<Proto::TransactionCoinFrom> inputs;
    std::copy(tx.inputs().begin(), tx.inputs().end(), std::back_inserter(inputs));
    serializerInput(inputs, data);
    // CoinData Output
    std::vector<Proto::TransactionCoinTo> outputs;
    std::copy(tx.outputs().begin(), tx.outputs().end(), std::back_inserter(outputs));
    serializerOutput(outputs, data);


    // Calc transaction hash
    Data txHash = calcTransactionDigest(data);
    auto priv = Address::importHexPrivateKey(input.private_key());
    auto transactionSignature = makeTransactionSignature(priv, txHash);

    encodeVarInt(transactionSignature.size(), data);
    std::copy(transactionSignature.begin(), transactionSignature.end(), std::back_inserter(data));

    return data;
}
