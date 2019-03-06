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

#include <TrustWalletCore/TWPrivateKey.h>

#include "TWJNI.h"
#include "PrivateKey.h"

jlong JNICALL Java_wallet_core_jni_PrivateKey_nativeCreate(JNIEnv *env, jclass thisClass) {
    struct TWPrivateKey *instance = TWPrivateKeyCreate();
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_PrivateKey_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWPrivateKey *instance = TWPrivateKeyCreateWithData(dataData);
    TWDataDelete(dataData);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_PrivateKey_nativeCreateCopy(JNIEnv *env, jclass thisClass, jobject key) {
    jclass keyClass = (*env)->GetObjectClass(env, key);
    jfieldID keyHandleFieldID = (*env)->GetFieldID(env, keyClass, "nativeHandle", "J");
    struct TWPrivateKey *keyInstance = (struct TWPrivateKey *) (*env)->GetLongField(env, key, keyHandleFieldID);
    struct TWPrivateKey *instance = TWPrivateKeyCreateCopy(keyInstance);
    (*env)->DeleteLocalRef(env, keyClass);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_PrivateKey_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWPrivateKeyDelete((struct TWPrivateKey *) handle);
}

jboolean JNICALL Java_wallet_core_jni_PrivateKey_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPrivateKeyIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKey_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWPrivateKeyData(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_PrivateKey_getPublicKeySecp256k1(JNIEnv *env, jobject thisObject, jboolean compressed) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWPublicKey *result = TWPrivateKeyGetPublicKeySecp256k1(instance, compressed);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jobject JNICALL Java_wallet_core_jni_PrivateKey_getPublicKeyEd25519(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWPublicKey *result = TWPrivateKeyGetPublicKeyEd25519(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKey_sign(JNIEnv *env, jobject thisObject, jbyteArray digest, jobject curve) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jclass curveClass = (*env)->GetObjectClass(env, curve);
    jmethodID curveValueMethodID = (*env)->GetMethodID(env, curveClass, "value", "()I");
    jint curveValue = (*env)->CallIntMethod(env, curve, curveValueMethodID);
    jbyteArray result = TWDataJByteArray(TWPrivateKeySign(instance, digestData, curveValue), env);

    TWDataDelete(digestData);
    (*env)->DeleteLocalRef(env, curveClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_PrivateKey_signAsDER(JNIEnv *env, jobject thisObject, jbyteArray digest, jobject curve) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jclass curveClass = (*env)->GetObjectClass(env, curve);
    jmethodID curveValueMethodID = (*env)->GetMethodID(env, curveClass, "value", "()I");
    jint curveValue = (*env)->CallIntMethod(env, curve, curveValueMethodID);
    jbyteArray result = TWDataJByteArray(TWPrivateKeySignAsDER(instance, digestData, curveValue), env);

    TWDataDelete(digestData);
    (*env)->DeleteLocalRef(env, curveClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

