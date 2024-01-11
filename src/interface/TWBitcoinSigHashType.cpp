// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "../Bitcoin/SigHashType.h"

bool TWBitcoinSigHashTypeIsSingle(enum TWBitcoinSigHashType type) {
    return TW::Bitcoin::hashTypeIsSingle(type);
}

bool TWBitcoinSigHashTypeIsNone(enum TWBitcoinSigHashType type) {
    return TW::Bitcoin::hashTypeIsNone(type);
}
