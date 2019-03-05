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

#include <TrustWalletCore/TWPrivateKeySecp256k1.h>

#include "TWJNI.h"
#include "PrivateKeySecp256k1.h"

jlong JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_nativeCreate(JNIEnv *env, jclass thisClass) {
    struct TWPrivateKeySecp256k1 *instance = TWPrivateKeySecp256k1Create();
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWPrivateKeySecp256k1 *instance = TWPrivateKeySecp256k1CreateWithData(dataData);
    TWDataDelete(dataData);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_nativeCreateCopy(JNIEnv *env, jclass thisClass, jobject key) {
    jclass keyClass = (*env)->GetObjectClass(env, key);
    jfieldID keyHandleFieldID = (*env)->GetFieldID(env, keyClass, "nativeHandle", "J");
    struct TWPrivateKeySecp256k1 *keyInstance = (struct TWPrivateKeySecp256k1 *) (*env)->GetLongField(env, key, keyHandleFieldID);
    struct TWPrivateKeySecp256k1 *instance = TWPrivateKeySecp256k1CreateCopy(keyInstance);
    (*env)->DeleteLocalRef(env, keyClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWPrivateKeySecp256k1Delete((struct TWPrivateKeySecp256k1 *) handle);
}

jboolean JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPrivateKeySecp256k1IsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeySecp256k1 *instance = (struct TWPrivateKeySecp256k1 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWPrivateKeySecp256k1Data(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_getPublicKey(JNIEnv *env, jobject thisObject, jboolean compressed) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeySecp256k1 *instance = (struct TWPrivateKeySecp256k1 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWPublicKeySecp256k1 result = TWPrivateKeySecp256k1GetPublicKey(instance, compressed);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKeySecp256k1");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKeySecp256k1));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKeySecp256k1), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lwallet/core/jni/PublicKeySecp256k1;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_sign(JNIEnv *env, jobject thisObject, jbyteArray digest) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeySecp256k1 *instance = (struct TWPrivateKeySecp256k1 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jbyteArray result = TWDataJByteArray(TWPrivateKeySecp256k1Sign(instance, digestData), env);

    TWDataDelete(digestData);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKeySecp256k1_signAsDER(JNIEnv *env, jobject thisObject, jbyteArray digest) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKeySecp256k1 *instance = (struct TWPrivateKeySecp256k1 *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jbyteArray result = TWDataJByteArray(TWPrivateKeySecp256k1SignAsDER(instance, digestData), env);

    TWDataDelete(digestData);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

