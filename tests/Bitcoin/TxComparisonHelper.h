// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Bitcoin/Amount.h"
#include "Bitcoin/Transaction.h"
#include "Bitcoin/TransactionPlan.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWCoinType.h>

#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace TW;
using namespace TW::Bitcoin;

/// Build a dummy UTXO with the given amount
Proto::UnspentTransaction buildTestUTXO(int64_t amount);

/// Build a set of dummy UTXO with the given amounts
std::vector<Proto::UnspentTransaction> buildTestUTXOs(const std::vector<int64_t>& amounts);

Proto::SigningInput buildSigningInput(Amount amount, int byteFee, const std::vector<Proto::UnspentTransaction>& utxos, 
    bool useMaxAmount = false, enum TWCoinType coin = TWCoinTypeBitcoin);

/// Compare a set of selected UTXOs to the expected set of amounts.
/// Returns false on mismatch, and error is printed (stderr).
bool verifySelectedUTXOs(const std::vector<Proto::UnspentTransaction>& selected, const std::vector<int64_t>& expectedAmounts);

/// Compare a transaction plan against expected values (UTXO amounts, amount, fee, change is implicit).
/// Returns false on mismatch, and error is printed (stderr).
bool verifyPlan(const TransactionPlan& plan, const std::vector<int64_t>& utxoAmounts, int64_t outputAmount, int64_t fee, std::string error = "");

int64_t sumUTXOs(const std::vector<Proto::UnspentTransaction>& utxos);

struct EncodedTxSize {
    uint64_t segwit;
    uint64_t nonSegwit;
    uint64_t virtualBytes;
};
bool operator==(const EncodedTxSize& s1, const EncodedTxSize& s2);

/// Return the encoded size of the transaction, virtual and non-segwit, etc.
EncodedTxSize getEncodedTxSize(const Transaction& tx);

/// Validate the previously estimated transaction size (if available) with the actual transaction size.
/// Uses segwit byte size (virtual size).  Tolerance is estiamte-smaller and estimate-larger, like -1 and 20.
/// Returns false on mismatch, and error is printed (stderr).
bool validateEstimatedSize(const Transaction& tx, int smallerTolerance = -1, int biggerTolerance = 20);

/// Print out a transaction in a nice format, as structured hex strings.
void prettyPrintTransaction(const Transaction& tx, bool useWitnessFormat = true);
