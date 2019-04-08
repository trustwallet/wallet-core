// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <assert.h>
#include <vector>

#include "TWJNIData.h"

jbyteArray TWDataJByteArray(TWData *_Nonnull data, JNIEnv *env) {
    jbyteArray array = env->NewByteArray(TWDataSize(data));
    env->SetByteArrayRegion(array, 0, TWDataSize(data), (jbyte *) TWDataBytes(data));
    return array;
}

TWData *_Nonnull TWDataCreateWithJByteArray(JNIEnv *env, jbyteArray _Nonnull array) {
    jsize size = env->GetArrayLength(array);
    jbyte *bytes = env->GetByteArrayElements(array, nullptr);
    return TWDataCreateWithBytes((uint8_t *) bytes, size);
}
