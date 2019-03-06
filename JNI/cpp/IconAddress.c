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

#include <TrustWalletCore/TWIconAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TWJNI.h"
#include "IconAddress.h"

jlong JNICALL Java_wallet_core_jni_IconAddress_nativeCreateWithString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    struct TWIconAddress *instance = TWIconAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_IconAddress_nativeCreateWithKeyHash(JNIEnv *env, jclass thisClass, jbyteArray keyHash, jobject type) {
    TWData *keyHashData = TWDataCreateWithJByteArray(env, keyHash);
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    struct TWIconAddress *instance = TWIconAddressCreateWithKeyHash(keyHashData, typeValue);
    TWDataDelete(keyHashData);
    (*env)->DeleteLocalRef(env, typeClass);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_IconAddress_nativeCreateWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey, jobject type) {
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyHandleFieldID = (*env)->GetFieldID(env, publicKeyClass, "nativeHandle", "J");
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) (*env)->GetLongField(env, publicKey, publicKeyHandleFieldID);
    jclass typeClass = (*env)->GetObjectClass(env, type);
    jmethodID typeValueMethodID = (*env)->GetMethodID(env, typeClass, "value", "()I");
    jint typeValue = (*env)->CallIntMethod(env, type, typeValueMethodID);
    struct TWIconAddress *instance = TWIconAddressCreateWithPublicKey(publicKeyInstance, typeValue);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    (*env)->DeleteLocalRef(env, typeClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_IconAddress_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWIconAddressDelete((struct TWIconAddress *) handle);
}

jboolean JNICALL Java_wallet_core_jni_IconAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsHandleFieldID = (*env)->GetFieldID(env, lhsClass, "nativeHandle", "J");
    struct TWIconAddress *lhsInstance = (struct TWIconAddress *) (*env)->GetLongField(env, lhs, lhsHandleFieldID);
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsHandleFieldID = (*env)->GetFieldID(env, rhsClass, "nativeHandle", "J");
    struct TWIconAddress *rhsInstance = (struct TWIconAddress *) (*env)->GetLongField(env, rhs, rhsHandleFieldID);
    jboolean resultValue = (jboolean) TWIconAddressEqual(lhsInstance, rhsInstance);

    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_IconAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWIconAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_wallet_core_jni_IconAddress_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWIconAddress *instance = (struct TWIconAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWIconAddressDescription(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_IconAddress_keyHash(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWIconAddress *instance = (struct TWIconAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWIconAddressKeyHash(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

