// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <assert.h>
#include <string.h>
#include "TWJNIString.h"

jstring _Nonnull TWStringJString(TWString *_Nonnull string, JNIEnv *env) {
    jstring result = env->NewStringUTF(TWStringUTF8Bytes(string));
    TWStringDelete(string);
    return result;
}

TWString *_Nonnull TWStringCreateWithJString(JNIEnv *env, jstring _Nonnull string) {
    const auto *chars = env->GetStringUTFChars(string, nullptr);
    const auto *twstring = TWStringCreateWithUTF8Bytes(chars);
    env->ReleaseStringUTFChars(string, chars);
    return twstring;
}
