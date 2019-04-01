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
#include <TrustWalletCore/TWStellarAddress.h>

#include "TWJNI.h"
#include "StellarAddress.h"

jlong JNICALL Java_wallet_core_jni_StellarAddress_nativeCreateWithString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    struct TWStellarAddress *instance = TWStellarAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_StellarAddress_nativeCreateWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey) {
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyHandleFieldID = (*env)->GetFieldID(env, publicKeyClass, "nativeHandle", "J");
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) (*env)->GetLongField(env, publicKey, publicKeyHandleFieldID);
    struct TWStellarAddress *instance = TWStellarAddressCreateWithPublicKey(publicKeyInstance);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_StellarAddress_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWStellarAddressDelete((struct TWStellarAddress *) handle);
}

jboolean JNICALL Java_wallet_core_jni_StellarAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsHandleFieldID = (*env)->GetFieldID(env, lhsClass, "nativeHandle", "J");
    struct TWStellarAddress *lhsInstance = (struct TWStellarAddress *) (*env)->GetLongField(env, lhs, lhsHandleFieldID);
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsHandleFieldID = (*env)->GetFieldID(env, rhsClass, "nativeHandle", "J");
    struct TWStellarAddress *rhsInstance = (struct TWStellarAddress *) (*env)->GetLongField(env, rhs, rhsHandleFieldID);
    jboolean resultValue = (jboolean) TWStellarAddressEqual(lhsInstance, rhsInstance);

    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_StellarAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWStellarAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_wallet_core_jni_StellarAddress_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStellarAddress *instance = (struct TWStellarAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWStellarAddressDescription(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_StellarAddress_keyHash(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStellarAddress *instance = (struct TWStellarAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWStellarAddressKeyHash(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

