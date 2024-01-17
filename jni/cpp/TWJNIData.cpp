// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
