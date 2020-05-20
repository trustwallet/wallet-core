// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Bitcoin/Amount.h"
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
/// Returns "" on match, mismatch description on mismatch.
std::string verifySelectedUTXOs(const std::vector<Proto::UnspentTransaction>& selected, const std::vector<int64_t>& expectedAmounts);

/// Compare a transaction plan against expected values (UTXO amounts, amount, fee, change is implicit).
/// Returns "" on match, mismatch description on mismatch.
std::string verifyPlan(const TransactionPlan& plan, const std::vector<int64_t>& utxoAmounts, int64_t outputAmount, int64_t fee);

int64_t sumUTXOs(const std::vector<Proto::UnspentTransaction>& utxos);
