// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <jni.h>
#include <stdio.h>
#include <string.h>

#include "AnySigner.h"
#include "TWJNI.h"

jbyteArray JNICALL Java_com_trustwallet_core_AnySigner_nativeSign(JNIEnv *env, jobject thisClass, jbyteArray input, uint32_t coin) {
    TWData *inputData = TWDataCreateWithJByteArray(env, input);
    TWData *outputData = TWAnySignerSign(inputData, coin);
    jbyteArray resultData = TWDataJByteArray(outputData, env);
    TWDataDelete(inputData);
    return resultData;
}

jboolean JNICALL Java_com_trustwallet_core_AnySigner_supportsJSON(JNIEnv *env, jobject thisClass, uint32_t coin) {
    return TWAnySignerSupportsJSON(coin);
}

jstring JNICALL Java_com_trustwallet_core_AnySigner_signJSON(JNIEnv *env, jobject thisClass, jstring json, jbyteArray key, uint32_t coin) {
    TWString *jsonString = TWStringCreateWithJString(env, json);
    TWData *keyData = TWDataCreateWithJByteArray(env, key);
    TWString *result = TWAnySignerSignJSON(jsonString, keyData, coin);
    TWDataDelete(keyData);
    TWStringDelete(jsonString);
    return TWStringJString(result, env);
}

jbyteArray JNICALL Java_com_trustwallet_core_AnySigner_nativePlan(JNIEnv *env, jobject thisClass, jbyteArray input, uint32_t coin) {
    TWData *inputData = TWDataCreateWithJByteArray(env, input);
    TWData *outputData = TWAnySignerPlan(inputData, coin);
    jbyteArray resultData = TWDataJByteArray(outputData, env);
    TWDataDelete(inputData);
    return resultData;
}
