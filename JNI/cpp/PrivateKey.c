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

#include <TrustWalletCore/TWPrivateKey.h>

#include "TWJNI.h"
#include "PrivateKey.h"

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_nativeCreate(JNIEnv *env, jclass thisClass) {
    struct TWPrivateKey *instance = TWPrivateKeyCreate();
    return (jlong) instance;
}

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWPrivateKey *instance = TWPrivateKeyCreateWithData(dataData);
    TWDataDelete(dataData);
    return (jlong) instance;
}

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_nativeCreateCopy(JNIEnv *env, jclass thisClass, jobject key) {
    jclass keyClass = (*env)->GetObjectClass(env, key);
    jfieldID keyHandleFieldID = (*env)->GetFieldID(env, keyClass, "nativeHandle", "J");
    struct TWPrivateKey *keyInstance = (struct TWPrivateKey *) (*env)->GetLongField(env, key, keyHandleFieldID);
    struct TWPrivateKey *instance = TWPrivateKeyCreateCopy(keyInstance);
    (*env)->DeleteLocalRef(env, keyClass);
    return (jlong) instance;
}

void JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWPrivateKeyDelete((struct TWPrivateKey *) handle);
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPrivateKeyIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWPrivateKeyData(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_getPublicKey(JNIEnv *env, jobject thisObject, jboolean compressed) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWPublicKey result = TWPrivateKeyGetPublicKey(instance, compressed);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "com/wallet/crypto/trustapp/jni/PublicKey");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKey));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKey), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lcom/wallet/crypto/trustapp/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_sign(JNIEnv *env, jobject thisObject, jbyteArray digest) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jbyteArray result = TWDataJByteArray(TWPrivateKeySign(instance, digestData), env);

    TWDataDelete(digestData);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_PrivateKey_signAsDER(JNIEnv *env, jobject thisObject, jbyteArray digest) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWPrivateKey *instance = (struct TWPrivateKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWData *digestData = TWDataCreateWithJByteArray(env, digest);
    jbyteArray result = TWDataJByteArray(TWPrivateKeySignAsDER(instance, digestData), env);

    TWDataDelete(digestData);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

