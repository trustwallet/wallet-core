// Copyright © 2017-2019 Trust Wallet.
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

#include <TrustWalletCore/TWBinanceSigner.h>

#include "TWJNI.h"
#include "BinanceSigner.h"

jlong JNICALL Java_wallet_core_jni_BinanceSigner_nativeCreate(JNIEnv *env, jclass thisClass, jobject input) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    struct TWBinanceSigner *instance = TWBinanceSignerCreate(inputData);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_BinanceSigner_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWBinanceSignerDelete((struct TWBinanceSigner *) handle);
}

jbyteArray JNICALL Java_wallet_core_jni_BinanceSigner_build(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWBinanceSigner *instance = (struct TWBinanceSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWBinanceSignerBuild(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

