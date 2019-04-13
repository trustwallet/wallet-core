// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoin.h>

bool TWSignatureHashTypeIsSingle(uint32_t type) {
    return (type & 0x1f) == TWSignatureHashTypeSingle;
}

bool TWSignatureHashTypeIsNone(uint32_t type) {
    return (type & 0x1f) == TWSignatureHashTypeNone;
}
