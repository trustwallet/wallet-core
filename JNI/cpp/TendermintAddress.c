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

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWTendermintAddress.h>

#include "TWJNI.h"
#include "TendermintAddress.h"

jlong JNICALL Java_wallet_core_jni_TendermintAddress_nativeCreateWithString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    struct TWTendermintAddress *instance = TWTendermintAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_TendermintAddress_nativeCreateWithKeyHash(JNIEnv *env, jclass thisClass, jobject hrp, jbyteArray keyHash) {
    jclass hrpClass = (*env)->GetObjectClass(env, hrp);
    jmethodID hrpValueMethodID = (*env)->GetMethodID(env, hrpClass, "value", "()I");
    jint hrpValue = (*env)->CallIntMethod(env, hrp, hrpValueMethodID);
    TWData *keyHashData = TWDataCreateWithJByteArray(env, keyHash);
    struct TWTendermintAddress *instance = TWTendermintAddressCreateWithKeyHash(hrpValue, keyHashData);
    (*env)->DeleteLocalRef(env, hrpClass);
    TWDataDelete(keyHashData);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_TendermintAddress_nativeCreateWithPublicKey(JNIEnv *env, jclass thisClass, jobject hrp, jobject publicKey) {
    jclass hrpClass = (*env)->GetObjectClass(env, hrp);
    jmethodID hrpValueMethodID = (*env)->GetMethodID(env, hrpClass, "value", "()I");
    jint hrpValue = (*env)->CallIntMethod(env, hrp, hrpValueMethodID);
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyHandleFieldID = (*env)->GetFieldID(env, publicKeyClass, "nativeHandle", "J");
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) (*env)->GetLongField(env, publicKey, publicKeyHandleFieldID);
    struct TWTendermintAddress *instance = TWTendermintAddressCreateWithPublicKey(hrpValue, publicKeyInstance);
    (*env)->DeleteLocalRef(env, hrpClass);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_TendermintAddress_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWTendermintAddressDelete((struct TWTendermintAddress *) handle);
}

jboolean JNICALL Java_wallet_core_jni_TendermintAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsHandleFieldID = (*env)->GetFieldID(env, lhsClass, "nativeHandle", "J");
    struct TWTendermintAddress *lhsInstance = (struct TWTendermintAddress *) (*env)->GetLongField(env, lhs, lhsHandleFieldID);
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsHandleFieldID = (*env)->GetFieldID(env, rhsClass, "nativeHandle", "J");
    struct TWTendermintAddress *rhsInstance = (struct TWTendermintAddress *) (*env)->GetLongField(env, rhs, rhsHandleFieldID);
    jboolean resultValue = (jboolean) TWTendermintAddressEqual(lhsInstance, rhsInstance);

    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_TendermintAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWTendermintAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_wallet_core_jni_TendermintAddress_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWTendermintAddress *instance = (struct TWTendermintAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWTendermintAddressDescription(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_TendermintAddress_hrp(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWTendermintAddress *instance = (struct TWTendermintAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    enum TWHRP result = TWTendermintAddressHRP(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/HRP");
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromValue", "(I)Lwallet/core/jni/HRP;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jint) result);
}

jbyteArray JNICALL Java_wallet_core_jni_TendermintAddress_keyHash(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWTendermintAddress *instance = (struct TWTendermintAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWTendermintAddressKeyHash(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

