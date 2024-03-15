// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Amount.h"
#include "DustCalculator.h"
#include "Transaction.h"
#include "UTXO.h"
#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "../proto/Bitcoin.pb.h"

#include <string>
#include <vector>
#include <map>
#include <optional>

namespace TW::Bitcoin {

/// Input for signing, info of an unsigned transaction
class SigningInput {
public:
    // Hash type to use when signing
    enum TWBitcoinSigHashType hashType = TWBitcoinSigHashTypeAll;

    // Amount to send.  Transaction created will have this amount in its output, 
    // except when use_max_amount is set, in that case this amount is not relevant, maximum possible amount will be used (max avail less fee).
    // If amount is equal or more than the available amount, also max amount will be used.
    Amount amount = 0;

    // Transaction fee per byte
    Amount byteFee = 0;

    // Recipient's address
    std::string toAddress;

    // Change address
    std::string changeAddress;

    // Available private keys
    std::vector<PrivateKey> privateKeys;

    // Available redeem scripts indexed by script hash
    std::map<std::string, Script> scripts;

    // Available unspent transaction outputs
    UTXOs utxos;

    // If sending max amount
    bool useMaxAmount = false;

    // If all input utxos
    bool useMaxUtxo = false;

    // If disable dust filter
    bool disableDustFilter = false;

    // Coin type (forks)
    TWCoinType coinType = TWCoinTypeBitcoin;

    // Optional transaction plan
    std::optional<TransactionPlan> plan;

    Data outputOpReturn;

    // Optional index of the OP_RETURN output in the transaction.
    // If not set, OP_RETURN output will be pushed as the latest output.
    MaybeIndex outputOpReturnIndex;

    uint32_t lockTime = 0;
    uint32_t time = 0;

    // Besides to_address and change_address,
    // we have other outputs that include address and value
    std::vector<std::pair<std::string, int64_t>> extraOutputs;

    // Total amount of the `extraOutputs`.
    Amount extraOutputsAmount = 0;

    DustCalculatorShared dustCalculator;

public:
    SigningInput();

    SigningInput(const Proto::SigningInput& input);
};

} // namespace TW::Bitcoin
