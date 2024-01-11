// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../Bitcoin/Transaction.h"

namespace TW::Groestlcoin {

struct Transaction : public Bitcoin::Transaction {
    Transaction() : Bitcoin::Transaction(1, 0, Hash::HasherSha256) {}
    Transaction(int32_t version, uint32_t lockTime = 0) :
        Bitcoin::Transaction(version, lockTime, Hash::HasherSha256) {}
};

} // namespace TW::Groestlcoin
