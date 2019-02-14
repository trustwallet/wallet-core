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

#include "TWJNI.h"
#include "PublicKey.h"

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWPublicKey));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWPublicKey *instance = (struct TWPublicKey *) bytesBuffer;
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean result = (jboolean) TWPublicKeyInitWithData(instance, dataData);
    TWDataDelete(dataData);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPublicKeyIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_recover(JNIEnv *env, jclass thisClass, jbyteArray signature, jbyteArray message) {
    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    struct TWPublicKey result = TWPublicKeyRecover(signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    jclass class = (*env)->FindClass(env, "com/wallet/crypto/trustapp/jni/PublicKey");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKey));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKey), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lcom/wallet/crypto/trustapp/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_isCompressed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKey *instance = (struct TWPublicKey *) bytesBuffer;

    jboolean resultValue = (jboolean) TWPublicKeyIsCompressed(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_compressed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKey *instance = (struct TWPublicKey *) bytesBuffer;

    struct TWPublicKey result = TWPublicKeyCompressed(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "com/wallet/crypto/trustapp/jni/PublicKey");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKey));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKey), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lcom/wallet/crypto/trustapp/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKey *instance = (struct TWPublicKey *) bytesBuffer;

    jbyteArray result = TWDataJByteArray(TWPublicKeyData(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKey *instance = (struct TWPublicKey *) bytesBuffer;

    jstring result = TWStringJString(TWPublicKeyDescription(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_PublicKey_verify(JNIEnv *env, jobject thisObject, jbyteArray signature, jbyteArray message) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKey *instance = (struct TWPublicKey *) bytesBuffer;

    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    jboolean resultValue = (jboolean) TWPublicKeyVerify(*instance, signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

