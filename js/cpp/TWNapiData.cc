// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWNapi.h"

using namespace Napi;

Uint8Array TWDataUint8Array(Env *_Nonnull env, TWData *_Nonnull data) {
    return Uint8Array::New(*env, TWDataSize(data), ArrayBuffer::New(*env, TWDataBytes(data), TWDataSize(data)), 0);
}

TWData *_Nonnull TWDataCreateWithUint8Array(Uint8Array data) {
    return TWDataCreateWithBytes((uint8_t*) data.Data(), data.ByteLength());
}
