// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <jni.h>
#include <stdio.h>
#include <string.h>

#include "AnySigner.h"
#include "TWJNI.h"

jbyteArray JNICALL Java_com_trustwallet_core_AnySigner_sign(JNIEnv *env, jclass thisClass, jbyteArray input, jobject coin) {
    JNI_CHECK_NULL_AND_RETURN_NULL(env, coin, "coin");
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    uint32_t coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);

    JNI_CHECK_NULL_AND_RETURN_NULL(env, input, "input");
    TWData *inputData = TWDataCreateWithJByteArray(env, input);
    TWData *outputData = TWAnySignerSign(inputData, coinValue);
    jbyteArray resultData = TWDataJByteArray(outputData, env);
    TWDataDelete(inputData);
    return resultData;
}

jboolean JNICALL Java_com_trustwallet_core_AnySigner_supportsJson(JNIEnv *env, jclass thisClass, jobject coin) {
    JNI_CHECK_NULL_AND_RETURN_ZERO(env, coin, "coin");
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    uint32_t coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    return TWAnySignerSupportsJSON(coinValue);
}

jstring JNICALL Java_com_trustwallet_core_AnySigner_signJson(JNIEnv *env, jclass thisClass, jstring json, jbyteArray key, jobject coin) {
    JNI_CHECK_NULL_AND_RETURN_NULL(env, coin, "coin");
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    uint32_t coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);

    JNI_CHECK_NULL_AND_RETURN_NULL(env, json, "json");
    TWString *jsonString = TWStringCreateWithJString(env, json);

    JNI_CHECK_NULL_AND_RETURN_NULL(env, key, "key");
    TWData *keyData = TWDataCreateWithJByteArray(env, key);
    TWString *result = TWAnySignerSignJSON(jsonString, keyData, coinValue);
    TWDataDelete(keyData);
    TWStringDelete(jsonString);
    return TWStringJString(result, env);
}

jbyteArray JNICALL Java_com_trustwallet_core_AnySigner_plan(JNIEnv *env, jclass thisClass, jbyteArray input, jobject coin) {
    JNI_CHECK_NULL_AND_RETURN_NULL(env, coin, "coin");
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    uint32_t coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);

    JNI_CHECK_NULL_AND_RETURN_NULL(env, input, "input");
    TWData *inputData = TWDataCreateWithJByteArray(env, input);
    TWData *outputData = TWAnySignerPlan(inputData, coinValue);
    jbyteArray resultData = TWDataJByteArray(outputData, env);
    TWDataDelete(inputData);
    return resultData;
}
