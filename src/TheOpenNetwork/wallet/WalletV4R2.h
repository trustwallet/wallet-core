// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Wallet.h"

namespace TW::TheOpenNetwork {

class WalletV4R2 : public Wallet {
public:
    WalletV4R2(PublicKey publicKey, int8_t workchainId);

private:
    [[nodiscard]] Cell::Ref createDataCell();
};

} // namespace TW::TheOpenNetwork
