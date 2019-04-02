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

#include <TrustWalletCore/TWDecredSigner.h>

#include "TWJNI.h"
#include "DecredSigner.h"

jlong JNICALL Java_wallet_core_jni_DecredSigner_nativeCreate(JNIEnv *env, jclass thisClass, jobject input) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    struct TWDecredSigner *instance = TWDecredSignerCreate(inputData);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_DecredSigner_nativeCreateWithPlan(JNIEnv *env, jclass thisClass, jobject input, jobject plan) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    jclass planClass = (*env)->GetObjectClass(env, plan);
    jmethodID planToByteArrayMethodID = (*env)->GetMethodID(env, planClass, "toByteArray", "()[B");
    jbyteArray planByteArray = (*env)->CallObjectMethod(env, plan, planToByteArrayMethodID);
    TWData *planData = TWDataCreateWithJByteArray(env, planByteArray);
    struct TWDecredSigner *instance = TWDecredSignerCreateWithPlan(inputData, planData);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);
    (*env)->DeleteLocalRef(env, planByteArray);
    (*env)->DeleteLocalRef(env, planClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_DecredSigner_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWDecredSignerDelete((struct TWDecredSigner *) handle);
}

jobject JNICALL Java_wallet_core_jni_DecredSigner_plan(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWDecredSigner *instance = (struct TWDecredSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray resultData = TWDataJByteArray(TWDecredSignerPlan(instance), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Bitcoin$TransactionPlan");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Bitcoin$TransactionPlan;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_DecredSigner_sign(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWDecredSigner *instance = (struct TWDecredSigner *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray resultData = TWDataJByteArray(TWDecredSignerSign(instance), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Common$Result");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Common$Result;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

