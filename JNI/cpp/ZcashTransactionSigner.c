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

#include <TrustWalletCore/TWZcashTransactionSigner.h>

#include "TWJNI.h"
#include "ZcashTransactionSigner.h"

jlong JNICALL Java_wallet_core_jni_ZcashTransactionSigner_nativeCreate(JNIEnv *env, jclass thisClass, jobject input) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    struct TWZcashTransactionSigner *instance = TWZcashTransactionSignerCreate(inputData);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_ZcashTransactionSigner_nativeCreateWithPlan(JNIEnv *env, jclass thisClass, jobject input, jobject plan) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    jclass planClass = (*env)->GetObjectClass(env, plan);
    jmethodID planToByteArrayMethodID = (*env)->GetMethodID(env, planClass, "toByteArray", "()[B");
    jbyteArray planByteArray = (*env)->CallObjectMethod(env, plan, planToByteArrayMethodID);
    TWData *planData = TWDataCreateWithJByteArray(env, planByteArray);
    struct TWZcashTransactionSigner *instance = TWZcashTransactionSignerCreateWithPlan(inputData, planData);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);
    (*env)->DeleteLocalRef(env, planByteArray);
    (*env)->DeleteLocalRef(env, planClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_ZcashTransactionSigner_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWZcashTransactionSignerDelete((struct TWZcashTransactionSigner *) handle);
}

jobject JNICALL Java_wallet_core_jni_ZcashTransactionSigner_plan(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWZcashTransactionSigner *instance = (struct TWZcashTransactionSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray resultData = TWDataJByteArray(TWZcashTransactionSignerPlan(instance), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Proto$TransactionPlan");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Proto$TransactionPlan;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_ZcashTransactionSigner_sign(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWZcashTransactionSigner *instance = (struct TWZcashTransactionSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray resultData = TWDataJByteArray(TWZcashTransactionSignerSign(instance), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Common$Result");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Common$Result;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

