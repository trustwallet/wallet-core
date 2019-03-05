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
#include <TrustWalletCore/TWTezosAddress.h>

#include "TWJNI.h"
#include "TezosAddress.h"

jbyteArray JNICALL Java_wallet_core_jni_TezosAddress_initWithString(JNIEnv *env, jclass thisClass, jstring string) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWTezosAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWTezosAddress *instance = (struct TWTezosAddress *) bytesBuffer;
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean result = (jboolean) TWTezosAddressInitWithString(instance, stringString);
    TWStringDelete(stringString);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_wallet_core_jni_TezosAddress_initWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWTezosAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWTezosAddress *instance = (struct TWTezosAddress *) bytesBuffer;
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyBytesFieldID = (*env)->GetFieldID(env, publicKeyClass, "bytes", "[B");
    jbyteArray publicKeyBytesArray = (*env)->GetObjectField(env, publicKey, publicKeyBytesFieldID);
    jbyte* publicKeyBytesBuffer = (*env)->GetByteArrayElements(env, publicKeyBytesArray, NULL);
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) publicKeyBytesBuffer;
    jboolean result = (jboolean) TWTezosAddressInitWithPublicKey(instance, *publicKeyInstance);
    (*env)->ReleaseByteArrayElements(env, publicKeyBytesArray, publicKeyBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, publicKeyBytesArray);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jstring JNICALL Java_wallet_core_jni_TezosAddress_description(JNIEnv *env, jclass thisClass, jobject address) {
    jclass addressClass = (*env)->GetObjectClass(env, address);
    jfieldID addressHandleFieldID = (*env)->GetFieldID(env, addressClass, "nativeHandle", "J");
    struct TWTezosAddress *addressInstance = (struct TWTezosAddress *) (*env)->GetLongField(env, address, addressHandleFieldID);
    jstring result = TWStringJString(TWTezosAddressDescription(addressInstance), env);

    (*env)->DeleteLocalRef(env, addressClass);

    return result;
}

jboolean JNICALL Java_wallet_core_jni_TezosAddress_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWTezosAddressIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_TezosAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWTezosAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_TezosAddress_data(JNIEnv *env, jclass thisClass, jobject address) {
    jclass addressClass = (*env)->GetObjectClass(env, address);
    jfieldID addressHandleFieldID = (*env)->GetFieldID(env, addressClass, "nativeHandle", "J");
    struct TWTezosAddress *addressInstance = (struct TWTezosAddress *) (*env)->GetLongField(env, address, addressHandleFieldID);
    jbyteArray result = TWDataJByteArray(TWTezosAddressData(addressInstance), env);

    (*env)->DeleteLocalRef(env, addressClass);

    return result;
}

