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

#include <TrustWalletCore/TWBitcoinTransactionSigner.h>

#include "TWJNI.h"
#include "BitcoinTransactionSigner.h"

jlong JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_nativeCreate(JNIEnv *env, jclass thisClass, jobject input) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    struct TWBitcoinTransactionSigner *instance = TWBitcoinTransactionSignerCreate(inputData);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWBitcoinTransactionSignerDelete((struct TWBitcoinTransactionSigner *) handle);
}

jobject JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_plan(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWBitcoinTransactionSigner *instance = (struct TWBitcoinTransactionSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray resultData = TWDataJByteArray(TWBitcoinTransactionSignerPlan(instance), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Proto$TransactionPlan");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Proto$TransactionPlan;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_sign(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWBitcoinTransactionSigner *instance = (struct TWBitcoinTransactionSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray resultData = TWDataJByteArray(TWBitcoinTransactionSignerSign(instance), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Common$Result");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Common$Result;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

