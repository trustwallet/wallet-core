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

#include <TrustWalletCore/TWBech32Address.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TWJNI.h"
#include "Bech32Address.h"

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_nativeCreateWithString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    struct TWBech32Address *instance = TWBech32AddressCreateWithString(stringString);
    TWStringDelete(stringString);
    return (jlong) instance;
}

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_nativeCreateWithData(JNIEnv *env, jclass thisClass, jobject hrp, jbyteArray data) {
    jclass hrpClass = (*env)->GetObjectClass(env, hrp);
    jmethodID hrpValueMethodID = (*env)->GetMethodID(env, hrpClass, "value", "()I");
    jint hrpValue = (*env)->CallIntMethod(env, hrp, hrpValueMethodID);
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    struct TWBech32Address *instance = TWBech32AddressCreateWithData(hrpValue, dataData);
    (*env)->DeleteLocalRef(env, hrpClass);
    TWDataDelete(dataData);
    return (jlong) instance;
}

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_nativeCreateWithPublicKey(JNIEnv *env, jclass thisClass, jobject hrp, jobject publicKey) {
    jclass hrpClass = (*env)->GetObjectClass(env, hrp);
    jmethodID hrpValueMethodID = (*env)->GetMethodID(env, hrpClass, "value", "()I");
    jint hrpValue = (*env)->CallIntMethod(env, hrp, hrpValueMethodID);
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyBytesFieldID = (*env)->GetFieldID(env, publicKeyClass, "bytes", "[B");
    jbyteArray publicKeyBytesArray = (*env)->GetObjectField(env, publicKey, publicKeyBytesFieldID);
    jbyte* publicKeyBytesBuffer = (*env)->GetByteArrayElements(env, publicKeyBytesArray, NULL);
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) publicKeyBytesBuffer;
    struct TWBech32Address *instance = TWBech32AddressCreateWithPublicKey(hrpValue, *publicKeyInstance);
    (*env)->DeleteLocalRef(env, hrpClass);
    (*env)->ReleaseByteArrayElements(env, publicKeyBytesArray, publicKeyBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, publicKeyBytesArray);
    (*env)->DeleteLocalRef(env, publicKeyClass);
    return (jlong) instance;
}

void JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWBech32AddressDelete((struct TWBech32Address *) handle);
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsHandleFieldID = (*env)->GetFieldID(env, lhsClass, "nativeHandle", "J");
    struct TWBech32Address *lhsInstance = (struct TWBech32Address *) (*env)->GetLongField(env, lhs, lhsHandleFieldID);
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsHandleFieldID = (*env)->GetFieldID(env, rhsClass, "nativeHandle", "J");
    struct TWBech32Address *rhsInstance = (struct TWBech32Address *) (*env)->GetLongField(env, rhs, rhsHandleFieldID);
    jboolean resultValue = (jboolean) TWBech32AddressEqual(lhsInstance, rhsInstance);

    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWBech32AddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWBech32Address *instance = (struct TWBech32Address *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWBech32AddressDescription(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_hrp(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWBech32Address *instance = (struct TWBech32Address *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jobject resultValue = (jobject) TWBech32AddressHRP(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_Bech32Address_witnessProgram(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWBech32Address *instance = (struct TWBech32Address *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWBech32AddressWitnessProgram(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

