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

#include <TrustWalletCore/TWPrivateKeyEd25519.h>

#include "TWJNI.h"
#include "PrivateKeyEd25519.h"

jlong JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeCreate(JNIEnv *env, jclass thisClass) {
    struct TWPrivateKeyEd25519 *instance = TWPrivateKeyEd25519Create();
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWPrivateKeyEd25519 *instance = TWPrivateKeyEd25519CreateWithData(dataData);
    TWDataDelete(dataData);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeCreateCopy(JNIEnv *env, jclass thisClass, jobject key) {
    jclass keyClass = (*env)->GetObjectClass(env, key);
    jfieldID keyHandleFieldID = (*env)->GetFieldID(env, keyClass, "nativeHandle", "J");
    struct TWPrivateKeyEd25519 *keyInstance = (struct TWPrivateKeyEd25519 *) (*env)->GetLongField(env, key, keyHandleFieldID);
    struct TWPrivateKeyEd25519 *instance = TWPrivateKeyEd25519CreateCopy(keyInstance);
    (*env)->DeleteLocalRef(env, keyClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWPrivateKeyEd25519Delete((struct TWPrivateKeyEd25519 *) handle);
}

jboolean JNICALL Java_wallet_core_jni_PrivateKeyEd25519_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPrivateKeyEd25519IsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKeyEd25519_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeyEd25519 *instance = (struct TWPrivateKeyEd25519 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWPrivateKeyEd25519Data(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_PrivateKeyEd25519_getPublicKey(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeyEd25519 *instance = (struct TWPrivateKeyEd25519 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWPublicKeyEd25519 result = TWPrivateKeyEd25519GetPublicKey(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKeyEd25519");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKeyEd25519));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKeyEd25519), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lwallet/core/jni/PublicKeyEd25519;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKeyEd25519_sign(JNIEnv *env, jobject thisObject, jbyteArray digest) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeyEd25519 *instance = (struct TWPrivateKeyEd25519 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jbyteArray result = TWDataJByteArray(TWPrivateKeyEd25519Sign(instance, digestData), env);

    TWDataDelete(digestData);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

