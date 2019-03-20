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

#include "TWJNI.h"
#include "PublicKey.h"

jlong JNICALL Java_wallet_core_jni_PublicKey_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWPublicKey *instance = TWPublicKeyCreateWithData(dataData);
    TWDataDelete(dataData);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_PublicKey_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWPublicKeyDelete((struct TWPublicKey *) handle);
}

jboolean JNICALL Java_wallet_core_jni_PublicKey_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPublicKeyIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jobject JNICALL Java_wallet_core_jni_PublicKey_recover(JNIEnv *env, jclass thisClass, jbyteArray signature, jbyteArray message) {
    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    struct TWPublicKey *result = TWPublicKeyRecover(signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jboolean JNICALL Java_wallet_core_jni_PublicKey_isCompressed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPublicKey *instance = (struct TWPublicKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jboolean resultValue = (jboolean) TWPublicKeyIsCompressed(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jobject JNICALL Java_wallet_core_jni_PublicKey_compressed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPublicKey *instance = (struct TWPublicKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWPublicKey *result = TWPublicKeyCompressed(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jbyteArray JNICALL Java_wallet_core_jni_PublicKey_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPublicKey *instance = (struct TWPublicKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWPublicKeyData(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_PublicKey_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPublicKey *instance = (struct TWPublicKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWPublicKeyDescription(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jboolean JNICALL Java_wallet_core_jni_PublicKey_verify(JNIEnv *env, jobject thisObject, jbyteArray signature, jbyteArray message) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPublicKey *instance = (struct TWPublicKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    jboolean resultValue = (jboolean) TWPublicKeyVerify(instance, signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

