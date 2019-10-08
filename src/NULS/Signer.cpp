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

Signer::Signer(Proto::SigningInput& input) : input(input) {
    Proto::TransactionCoinFrom coinFrom;
    coinFrom.set_from_address(input.from_address());
    coinFrom.set_assets_chainid(input.chain_id());
    coinFrom.set_assets_id(input.idassets_id());
    //need to update with amount + fee
    coinFrom.set_id_amount(input.amount());
    coinFrom.set_nonce(input.nonce());
    //default unlocked
    coinFrom.set_locked(0);
    *tx.mutable_input() = coinFrom;

    Proto::TransactionCoinTo coinTo;
    coinTo.set_to_address(input.to_address());
    coinTo.set_id_amount(input.amount());
    coinTo.set_assets_chainid(input.chain_id());
    coinTo.set_assets_id(input.idassets_id());
    coinTo.set_lock_time(0);
    *tx.mutable_output() = coinTo;

    tx.set_remark(input.remark());
    tx.set_type(2);
    tx.set_timestamp(input.timestamp());
    tx.set_tx_data("");
}

Data Signer::sign() const {
    if (input.private_key().empty()) {
        throw std::invalid_argument("Must have private key string");
    }

    uint32_t txSize = CalculatorTransactionSize(1, 1, static_cast<uint32_t>(tx.remark().size()));
    uint256_t fee = (uint256_t)CalculatorTransactionFee(txSize);
    uint256_t txAmount = load(input.amount());
    uint256_t balance = load(input.balance());
    uint256_t fromAmount = txAmount + fee;
    if (fromAmount > balance) {
        throw std::invalid_argument("User account balance not sufficient");
    }

    Proto::TransactionCoinFrom& coinFrom = (Proto::TransactionCoinFrom&)tx.input();
    Data amount;
    amount = store(fromAmount);
    std::reverse(amount.begin(), amount.end());
    std::string amountStr;
    amountStr.insert(amountStr.begin(), amount.begin(), amount.end());
    amountStr.append((unsigned long)(amount.capacity() - amount.size()), '\0');
    coinFrom.set_id_amount(amountStr);

    Proto::TransactionCoinTo& coinTo = (Proto::TransactionCoinTo&)tx.output();
    Data amountTo;
    amountTo = store(txAmount);
    std::reverse(amountTo.begin(), amountTo.end());
    std::string amountToStr;
    amountToStr.insert(amountToStr.begin(), amountTo.begin(), amountTo.end());
    amountToStr.append((unsigned long)(amountTo.capacity() - amountTo.size()), '\0');
    coinTo.set_id_amount(amountToStr);

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
    serializerInput(tx.input(), data);

    // CoinData Output
    serializerOutput(tx.output(), data);

    // Calc transaction hash
    Data txHash = calcTransactionDigest(data);
    auto priv = Address::importHexPrivateKey(input.private_key());
    auto transactionSignature = makeTransactionSignature(priv, txHash);
    encodeVarInt(transactionSignature.size(), data);
    std::copy(transactionSignature.begin(), transactionSignature.end(), std::back_inserter(data));

    return data;
}

uint32_t Signer::CalculatorTransactionSize(uint32_t inputCount, uint32_t outputCount, uint32_t remarkSize) const {
    uint32_t size = TRANSACTION_FIX_SIZE + TRANSACTION_SIG_MAX_SIZE + TRANSACTION_INPUT_SIZE * inputCount +
                        TRANSACTION_OUTPUT_SIZE * outputCount + remarkSize;
    return size;
}

uint64_t Signer::CalculatorTransactionFee(uint64_t size) const {
    uint64_t fee = (size / 1024) * MIN_PRICE_PRE_1024_BYTES;
    if (size % 1024 > 0) {
        fee += MIN_PRICE_PRE_1024_BYTES;
    }
    return fee;
}