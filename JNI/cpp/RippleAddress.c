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

#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWRippleAddress.h>

#include "TWJNI.h"
#include "RippleAddress.h"

jlong JNICALL Java_wallet_core_jni_RippleAddress_nativeCreateWithString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    struct TWRippleAddress *instance = TWRippleAddressCreateWithString(stringString);
    TWStringDelete(stringString);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_RippleAddress_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWRippleAddress *instance = TWRippleAddressCreateWithData(dataData);
    TWDataDelete(dataData);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_RippleAddress_nativeCreateWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey) {
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyBytesFieldID = (*env)->GetFieldID(env, publicKeyClass, "bytes", "[B");
    jbyteArray publicKeyBytesArray = (*env)->GetObjectField(env, publicKey, publicKeyBytesFieldID);
    jbyte* publicKeyBytesBuffer = (*env)->GetByteArrayElements(env, publicKeyBytesArray, NULL);
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) publicKeyBytesBuffer;
    struct TWRippleAddress *instance = TWRippleAddressCreateWithPublicKey(*publicKeyInstance);
    (*env)->ReleaseByteArrayElements(env, publicKeyBytesArray, publicKeyBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, publicKeyBytesArray);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_RippleAddress_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWRippleAddressDelete((struct TWRippleAddress *) handle);
}

jboolean JNICALL Java_wallet_core_jni_RippleAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsHandleFieldID = (*env)->GetFieldID(env, lhsClass, "nativeHandle", "J");
    struct TWRippleAddress *lhsInstance = (struct TWRippleAddress *) (*env)->GetLongField(env, lhs, lhsHandleFieldID);
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsHandleFieldID = (*env)->GetFieldID(env, rhsClass, "nativeHandle", "J");
    struct TWRippleAddress *rhsInstance = (struct TWRippleAddress *) (*env)->GetLongField(env, rhs, rhsHandleFieldID);
    jboolean resultValue = (jboolean) TWRippleAddressEqual(lhsInstance, rhsInstance);

    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_RippleAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWRippleAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_wallet_core_jni_RippleAddress_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWRippleAddress *instance = (struct TWRippleAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWRippleAddressDescription(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_RippleAddress_keyHash(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWRippleAddress *instance = (struct TWRippleAddress *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWRippleAddressKeyHash(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

