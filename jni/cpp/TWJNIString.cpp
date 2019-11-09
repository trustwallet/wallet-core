// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <assert.h>
#include <string.h>
#include "TWJNIString.h"

jstring _Nonnull TWStringJString(TWString *_Nonnull string, JNIEnv *env) {
    return env->NewStringUTF(TWStringUTF8Bytes(string));
}

TWString *_Nonnull TWStringCreateWithJString(JNIEnv *env, jstring _Nonnull string) {
    auto chars = env->GetStringUTFChars(string, nullptr);
    auto twstring = TWStringCreateWithUTF8Bytes(chars);
    env->ReleaseStringUTFChars(string, chars);
    return twstring;
}
