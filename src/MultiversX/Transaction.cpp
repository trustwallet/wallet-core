// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Transaction.h"

namespace TW::MultiversX {

Transaction::Transaction()
    : nonce(0), sender(""), senderUsername(""), receiver(""), receiverUsername(""), guardian(""), value("0"), data(""), gasPrice(0), gasLimit(0), chainID(""), version(0), options(TransactionOptions::Default) {
}

bool Transaction::hasGuardian() const {
    return !guardian.empty();
}

} // namespace TW::MultiversX
