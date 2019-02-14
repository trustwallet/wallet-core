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

#include <TrustWalletCore/TWBitcoinCashAddress.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TWJNI.h"
#include "BitcoinCashAddress.h"

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_initWithString(JNIEnv *env, jclass thisClass, jstring string) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWBitcoinCashAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWBitcoinCashAddress *instance = (struct TWBitcoinCashAddress *) bytesBuffer;
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean result = (jboolean) TWBitcoinCashAddressInitWithString(instance, stringString);
    TWStringDelete(stringString);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWBitcoinCashAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWBitcoinCashAddress *instance = (struct TWBitcoinCashAddress *) bytesBuffer;
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean result = (jboolean) TWBitcoinCashAddressInitWithData(instance, dataData);
    TWDataDelete(dataData);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_initWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWBitcoinCashAddress));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWBitcoinCashAddress *instance = (struct TWBitcoinCashAddress *) bytesBuffer;
    jclass publicKeyClass = (*env)->GetObjectClass(env, publicKey);
    jfieldID publicKeyBytesFieldID = (*env)->GetFieldID(env, publicKeyClass, "bytes", "[B");
    jbyteArray publicKeyBytesArray = (*env)->GetObjectField(env, publicKey, publicKeyBytesFieldID);
    jbyte* publicKeyBytesBuffer = (*env)->GetByteArrayElements(env, publicKeyBytesArray, NULL);
    struct TWPublicKey *publicKeyInstance = (struct TWPublicKey *) publicKeyBytesBuffer;
    jboolean result = (jboolean) TWBitcoinCashAddressInitWithPublicKey(instance, *publicKeyInstance);
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

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsBytesFieldID = (*env)->GetFieldID(env, lhsClass, "bytes", "[B");
    jbyteArray lhsBytesArray = (*env)->GetObjectField(env, lhs, lhsBytesFieldID);
    jbyte* lhsBytesBuffer = (*env)->GetByteArrayElements(env, lhsBytesArray, NULL);
    struct TWBitcoinCashAddress *lhsInstance = (struct TWBitcoinCashAddress *) lhsBytesBuffer;
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsBytesFieldID = (*env)->GetFieldID(env, rhsClass, "bytes", "[B");
    jbyteArray rhsBytesArray = (*env)->GetObjectField(env, rhs, rhsBytesFieldID);
    jbyte* rhsBytesBuffer = (*env)->GetByteArrayElements(env, rhsBytesArray, NULL);
    struct TWBitcoinCashAddress *rhsInstance = (struct TWBitcoinCashAddress *) rhsBytesBuffer;
    jboolean resultValue = (jboolean) TWBitcoinCashAddressEqual(*lhsInstance, *rhsInstance);

    (*env)->ReleaseByteArrayElements(env, lhsBytesArray, lhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, lhsBytesArray);
    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->ReleaseByteArrayElements(env, rhsBytesArray, rhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, rhsBytesArray);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_isValid(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean resultValue = (jboolean) TWBitcoinCashAddressIsValid(dataData);

    TWDataDelete(dataData);

    return resultValue;
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean resultValue = (jboolean) TWBitcoinCashAddressIsValidString(stringString);

    TWStringDelete(stringString);

    return resultValue;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWBitcoinCashAddress *instance = (struct TWBitcoinCashAddress *) bytesBuffer;

    jstring result = TWStringJString(TWBitcoinCashAddressDescription(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWBitcoinCashAddress *instance = (struct TWBitcoinCashAddress *) bytesBuffer;

    jbyteArray result = TWDataJByteArray(TWBitcoinCashAddressData(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_legacyAddress(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWBitcoinCashAddress *instance = (struct TWBitcoinCashAddress *) bytesBuffer;

    struct TWBitcoinAddress result = TWBitcoinCashAddressLegacyAddress(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "com/wallet/crypto/trustapp/jni/BitcoinAddress");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWBitcoinAddress));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWBitcoinAddress), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lcom/wallet/crypto/trustapp/jni/BitcoinAddress;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

