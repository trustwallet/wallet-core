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

#include <TrustWalletCore/TWPublicKeySecp256k1.h>

#include "TWJNI.h"
#include "PublicKeySecp256k1.h"

jbyteArray JNICALL Java_wallet_core_jni_PublicKeySecp256k1_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWPublicKeySecp256k1));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWPublicKeySecp256k1 *instance = (struct TWPublicKeySecp256k1 *) bytesBuffer;
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean result = (jboolean) TWPublicKeySecp256k1InitWithData(instance, dataData);
    TWDataDelete(dataData);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jboolean JNICALL Java_wallet_core_jni_PublicKeySecp256k1_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPublicKeySecp256k1IsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jobject JNICALL Java_wallet_core_jni_PublicKeySecp256k1_recover(JNIEnv *env, jclass thisClass, jbyteArray signature, jbyteArray message) {
    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    struct TWPublicKeySecp256k1 result = TWPublicKeySecp256k1Recover(signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKeySecp256k1");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKeySecp256k1));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKeySecp256k1), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lwallet/core/jni/PublicKeySecp256k1;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jboolean JNICALL Java_wallet_core_jni_PublicKeySecp256k1_isCompressed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeySecp256k1 *instance = (struct TWPublicKeySecp256k1 *) bytesBuffer;

    jboolean resultValue = (jboolean) TWPublicKeySecp256k1IsCompressed(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jobject JNICALL Java_wallet_core_jni_PublicKeySecp256k1_compressed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeySecp256k1 *instance = (struct TWPublicKeySecp256k1 *) bytesBuffer;

    struct TWPublicKeySecp256k1 result = TWPublicKeySecp256k1Compressed(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKeySecp256k1");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKeySecp256k1));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKeySecp256k1), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lwallet/core/jni/PublicKeySecp256k1;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jbyteArray JNICALL Java_wallet_core_jni_PublicKeySecp256k1_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeySecp256k1 *instance = (struct TWPublicKeySecp256k1 *) bytesBuffer;

    jbyteArray result = TWDataJByteArray(TWPublicKeySecp256k1Data(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_PublicKeySecp256k1_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeySecp256k1 *instance = (struct TWPublicKeySecp256k1 *) bytesBuffer;

    jstring result = TWStringJString(TWPublicKeySecp256k1Description(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jboolean JNICALL Java_wallet_core_jni_PublicKeySecp256k1_verify(JNIEnv *env, jobject thisObject, jbyteArray signature, jbyteArray message) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeySecp256k1 *instance = (struct TWPublicKeySecp256k1 *) bytesBuffer;

    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    jboolean resultValue = (jboolean) TWPublicKeySecp256k1Verify(*instance, signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

