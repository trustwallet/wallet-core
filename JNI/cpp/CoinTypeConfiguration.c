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

#include <TrustWalletCore/TWCoinTypeConfiguration.h>

#include "TWJNI.h"
#include "CoinTypeConfiguration.h"

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_CoinTypeConfiguration_getSymbol(JNIEnv *env, jclass thisClass, jobject type) {
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    jstring result = TWStringJString(TWCoinTypeConfigurationGetSymbol(typeValue), env);

    (*env)->DeleteLocalRef(env, typeClass);

    return result;
}

jint JNICALL Java_com_wallet_crypto_trustapp_jni_CoinTypeConfiguration_getDecimals(JNIEnv *env, jclass thisClass, jobject type) {
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    jint resultValue = (jint) TWCoinTypeConfigurationGetDecimals(typeValue);

    (*env)->DeleteLocalRef(env, typeClass);

    return resultValue;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_CoinTypeConfiguration_getTransactionURL(JNIEnv *env, jclass thisClass, jobject type, jstring transactionID) {
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    TWString *transactionIDString = TWStringCreateWithJString(env, transactionID);
    jstring result = TWStringJString(TWCoinTypeConfigurationGetTransactionURL(typeValue, transactionIDString), env);

    (*env)->DeleteLocalRef(env, typeClass);
    TWStringDelete(transactionIDString);

    return result;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_CoinTypeConfiguration_getID(JNIEnv *env, jclass thisClass, jobject type) {
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    jstring result = TWStringJString(TWCoinTypeConfigurationGetID(typeValue), env);

    (*env)->DeleteLocalRef(env, typeClass);

    return result;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_CoinTypeConfiguration_getName(JNIEnv *env, jclass thisClass, jobject type) {
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    jstring result = TWStringJString(TWCoinTypeConfigurationGetName(typeValue), env);

    (*env)->DeleteLocalRef(env, typeClass);

    return result;
}

