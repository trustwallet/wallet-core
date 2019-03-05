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

#include <TrustWalletCore/TWPublicKeyEd25519.h>

#include "TWJNI.h"
#include "PublicKeyEd25519.h"

jbyteArray JNICALL Java_wallet_core_jni_PublicKeyEd25519_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWPublicKeyEd25519));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWPublicKeyEd25519 *instance = (struct TWPublicKeyEd25519 *) bytesBuffer;
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean result = (jboolean) TWPublicKeyEd25519InitWithData(instance, dataData);
    TWDataDelete(dataData);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jboolean JNICALL Java_wallet_core_jni_PublicKeyEd25519_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWPublicKeyEd25519IsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_PublicKeyEd25519_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeyEd25519 *instance = (struct TWPublicKeyEd25519 *) bytesBuffer;

    jbyteArray result = TWDataJByteArray(TWPublicKeyEd25519Data(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_PublicKeyEd25519_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeyEd25519 *instance = (struct TWPublicKeyEd25519 *) bytesBuffer;

    jstring result = TWStringJString(TWPublicKeyEd25519Description(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jboolean JNICALL Java_wallet_core_jni_PublicKeyEd25519_verify(JNIEnv *env, jobject thisObject, jbyteArray signature, jbyteArray message) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWPublicKeyEd25519 *instance = (struct TWPublicKeyEd25519 *) bytesBuffer;

    TWData *signatureData = TWDataCreateWithJByteArray(env, signature);
    TWData *messageData = TWDataCreateWithJByteArray(env, message);
    jboolean resultValue = (jboolean) TWPublicKeyEd25519Verify(*instance, signatureData, messageData);

    TWDataDelete(signatureData);
    TWDataDelete(messageData);

    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

