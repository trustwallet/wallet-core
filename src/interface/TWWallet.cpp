// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWWallet.h>

#include "../Address.h"

bool TWWalletValidate(TWString *_Nonnull string, enum TWCoinType coin) {
    auto& s = *reinterpret_cast<const std::string*>(string);
    return TW::validate(s, coin);
}
