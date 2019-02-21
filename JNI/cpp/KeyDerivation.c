// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <jni.h>
#include <stdio.h>
#include <string.h>

#include <TrustWalletCore/TWKeyDerivation.h>

#include "TWJNI.h"
#include "KeyDerivation.h"

jbyteArray JNICALL Java_wallet_core_jni_KeyDerivation_scrypt(JNIEnv *env, jclass thisClass, jstring password, jbyteArray salt, jlong n, jint r, jint p, jsize keyLength) {
    TWString *passwordString = TWStringCreateWithJString(env, password);
    TWData *saltData = TWDataCreateWithJByteArray(env, salt);
    jbyteArray result = TWDataJByteArray(TWKeyDerivationScrypt(passwordString, saltData, n, r, p, keyLength), env);

    TWStringDelete(passwordString);
    TWDataDelete(saltData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_KeyDerivation_pbkdf2256(JNIEnv *env, jclass thisClass, jstring password, jbyteArray salt, jint iterations, jsize keyLength) {
    TWString *passwordString = TWStringCreateWithJString(env, password);
    TWData *saltData = TWDataCreateWithJByteArray(env, salt);
    jbyteArray result = TWDataJByteArray(TWKeyDerivationPBKDF2_256(passwordString, saltData, iterations, keyLength), env);

    TWStringDelete(passwordString);
    TWDataDelete(saltData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_KeyDerivation_pbkdf2512(JNIEnv *env, jclass thisClass, jstring password, jbyteArray salt, jint iterations, jsize keyLength) {
    TWString *passwordString = TWStringCreateWithJString(env, password);
    TWData *saltData = TWDataCreateWithJByteArray(env, salt);
    jbyteArray result = TWDataJByteArray(TWKeyDerivationPBKDF2_512(passwordString, saltData, iterations, keyLength), env);

    TWStringDelete(passwordString);
    TWDataDelete(saltData);

    return result;
}

