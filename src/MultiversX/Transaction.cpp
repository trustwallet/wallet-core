// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Transaction.h"

namespace TW::MultiversX {

Transaction::Transaction()
    : nonce(0), sender(""), senderUsername(""), receiver(""), receiverUsername(""), guardian(""), value("0"), data(""), gasPrice(0), gasLimit(0), chainID(""), version(0), options(TransactionOptions::Default) {
}

bool Transaction::hasGuardian() const {
    return !guardian.empty();
}

} // namespace TW::MultiversX
