// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Cell.h"
#include "../Data.h"
#include "../PublicKey.h"

namespace TW::TON {

using namespace TW;

/// Smart contract support, limited
class Contract {
public:
    /// Return the (compiled) smart contract of a wallet account
    static Data walletContractDefault();

    /// Create a StateInit structure for account initialization
    static Cell createStateInit(const PublicKey& pubkey);
};

} // namespace TW::TON
