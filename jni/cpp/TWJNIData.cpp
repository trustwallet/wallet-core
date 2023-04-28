// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <assert.h>
#include <vector>

#include "TWJNIData.h"

jbyteArray TWDataJByteArray(TWData *_Nonnull data, JNIEnv *env) {
    auto dataSize = static_cast<jsize>(TWDataSize(data));
    jbyteArray array = env->NewByteArray(dataSize);
    env->SetByteArrayRegion(array, 0, dataSize, (jbyte *) TWDataBytes(data));
    TWDataDelete(data);
    return array;
}

TWData *_Nonnull TWDataCreateWithJByteArray(JNIEnv *env, jbyteArray _Nonnull array) {
    jsize size = env->GetArrayLength(array);
    jbyte *bytes = env->GetByteArrayElements(array, nullptr);
    return TWDataCreateWithBytes((uint8_t *) bytes, size);
}
