// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinSigHashType.h>
#include "../Bitcoin/SigHashType.h"

bool TWBitcoinSigHashTypeIsSingle(enum TWBitcoinSigHashType type) {
    return TW::Bitcoin::hashTypeIsSingle(type);
}

bool TWBitcoinSigHashTypeIsNone(enum TWBitcoinSigHashType type) {
    return TW::Bitcoin::hashTypeIsNone(type);
}
