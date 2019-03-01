// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

@import Foundation;

#include "TWFoundationData.h"

TWData *_Nonnull TWDataCreateWithNSData(NSData *data) {
    return TWDataCreateWithBytes(data.bytes, data.length);
}

NSData *_Nonnull TWDataNSData(TWData *_Nonnull data) {
    return [[NSData alloc] initWithBytes:TWDataBytes(data) length:TWDataSize(data)];
}
