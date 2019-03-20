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

#include <TrustWalletCore/TWBitcoinAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TWJNI.h"
#include "BitcoinAddress.h"

jbyteArray JNICALL Java_wallet_core_jni_BitcoinAddress_initWithString(JNIEnv *env, jclass thisClass, jstring string) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWBitcoinAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWBitcoinAddress *instance = (struct TWBitcoinAddress *) bytesBuffer;
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean result = (jboolean) TWBitcoinAddressInitWithString(instance, stringString);
    TWStringDelete(stringString);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_wallet_core_jni_BitcoinAddress_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWBitcoinAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWBitcoinAddress *instance = (struct TWBitcoinAddress *) bytesBuffer;
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean result = (jboolean) TWBitcoinAddressInitWithData(instance, dataData);
    TWDataDelete(dataData);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_wallet_core_jni_BitcoinAddress_initWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey, jchar prefix) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWBitcoinAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWBitcoinAddress *instance = (struct TWBitcoinAddress *) bytesBuffer;
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyHandleFieldID = (*env)->GetFieldID(env, publicKeyClass, "nativeHandle", "J");
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) (*env)->GetLongField(env, publicKey, publicKeyHandleFieldID);
    TWBitcoinAddressInitWithPublicKey(instance, publicKeyInstance, prefix);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    return array;
}

jboolean JNICALL Java_wallet_core_jni_BitcoinAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsBytesFieldID = (*env)->GetFieldID(env, lhsClass, "bytes", "[B");
    jbyteArray lhsBytesArray = (*env)->GetObjectField(env, lhs, lhsBytesFieldID);
    jbyte* lhsBytesBuffer = (*env)->GetByteArrayElements(env, lhsBytesArray, NULL);
    struct TWBitcoinAddress *lhsInstance = (struct TWBitcoinAddress *) lhsBytesBuffer;
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsBytesFieldID = (*env)->GetFieldID(env, rhsClass, "bytes", "[B");
    jbyteArray rhsBytesArray = (*env)->GetObjectField(env, rhs, rhsBytesFieldID);
    jbyte* rhsBytesBuffer = (*env)->GetByteArrayElements(env, rhsBytesArray, NULL);
    struct TWBitcoinAddress *rhsInstance = (struct TWBitcoinAddress *) rhsBytesBuffer;
    jboolean resultValue = (jboolean) TWBitcoinAddressEqual(*lhsInstance, *rhsInstance);

    (*env)->ReleaseByteArrayElements(env, lhsBytesArray, lhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, lhsBytesArray);
    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->ReleaseByteArrayElements(env, rhsBytesArray, rhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, rhsBytesArray);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_BitcoinAddress_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWBitcoinAddressIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jboolean JNICALL Java_wallet_core_jni_BitcoinAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWBitcoinAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_wallet_core_jni_BitcoinAddress_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWBitcoinAddress *instance = (struct TWBitcoinAddress *) bytesBuffer;

    jstring result = TWStringJString(TWBitcoinAddressDescription(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_BitcoinAddress_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWBitcoinAddress *instance = (struct TWBitcoinAddress *) bytesBuffer;

    jbyteArray result = TWDataJByteArray(TWBitcoinAddressData(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

