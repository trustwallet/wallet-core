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

#include <TrustWalletCore/TWUInt256.h>

#include "TWJNI.h"
#include "UInt256.h"

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWUInt256));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jboolean result = (jboolean) TWUInt256InitWithData(instance, dataData);
    TWDataDelete(dataData);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_initWithString(JNIEnv *env, jclass thisClass, jstring string) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWUInt256));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;
    TWString *stringString = TWStringCreateWithJString(env, string);
    jboolean result = (jboolean) TWUInt256InitWithString(instance, stringString);
    TWStringDelete(stringString);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    if (result) {
        return array;
    } else {
        (*env)->DeleteLocalRef(env, array);
        return NULL;
    }
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_initWithUInt32(JNIEnv *env, jclass thisClass, jint value) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWUInt256));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;
    TWUInt256InitWithUInt32(instance, value);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    return array;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_initWithUInt64(JNIEnv *env, jclass thisClass, jlong value) {
    jbyteArray array = (*env)->NewByteArray(env, sizeof(struct TWUInt256));
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, array, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;
    TWUInt256InitWithUInt64(instance, value);
    (*env)->ReleaseByteArrayElements(env, array, bytesBuffer, 0);

    return array;
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_zero(JNIEnv *env, jclass thisClass) {
    struct TWUInt256 result = TWUInt256Zero();


    jclass class = (*env)->FindClass(env, "com/wallet/crypto/trustapp/jni/UInt256");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWUInt256));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWUInt256), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lcom/wallet/crypto/trustapp/jni/UInt256;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_one(JNIEnv *env, jclass thisClass) {
    struct TWUInt256 result = TWUInt256One();


    jclass class = (*env)->FindClass(env, "com/wallet/crypto/trustapp/jni/UInt256");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWUInt256));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWUInt256), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lcom/wallet/crypto/trustapp/jni/UInt256;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsBytesFieldID = (*env)->GetFieldID(env, lhsClass, "bytes", "[B");
    jbyteArray lhsBytesArray = (*env)->GetObjectField(env, lhs, lhsBytesFieldID);
    jbyte* lhsBytesBuffer = (*env)->GetByteArrayElements(env, lhsBytesArray, NULL);
    struct TWUInt256 *lhsInstance = (struct TWUInt256 *) lhsBytesBuffer;
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsBytesFieldID = (*env)->GetFieldID(env, rhsClass, "bytes", "[B");
    jbyteArray rhsBytesArray = (*env)->GetObjectField(env, rhs, rhsBytesFieldID);
    jbyte* rhsBytesBuffer = (*env)->GetByteArrayElements(env, rhsBytesArray, NULL);
    struct TWUInt256 *rhsInstance = (struct TWUInt256 *) rhsBytesBuffer;
    jboolean resultValue = (jboolean) TWUInt256Equal(*lhsInstance, *rhsInstance);

    (*env)->ReleaseByteArrayElements(env, lhsBytesArray, lhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, lhsBytesArray);
    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->ReleaseByteArrayElements(env, rhsBytesArray, rhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, rhsBytesArray);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_less(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs) {
    jclass lhsClass = (*env)->GetObjectClass(env, lhs);
    jfieldID lhsBytesFieldID = (*env)->GetFieldID(env, lhsClass, "bytes", "[B");
    jbyteArray lhsBytesArray = (*env)->GetObjectField(env, lhs, lhsBytesFieldID);
    jbyte* lhsBytesBuffer = (*env)->GetByteArrayElements(env, lhsBytesArray, NULL);
    struct TWUInt256 *lhsInstance = (struct TWUInt256 *) lhsBytesBuffer;
    jclass rhsClass = (*env)->GetObjectClass(env, rhs);
    jfieldID rhsBytesFieldID = (*env)->GetFieldID(env, rhsClass, "bytes", "[B");
    jbyteArray rhsBytesArray = (*env)->GetObjectField(env, rhs, rhsBytesFieldID);
    jbyte* rhsBytesBuffer = (*env)->GetByteArrayElements(env, rhsBytesArray, NULL);
    struct TWUInt256 *rhsInstance = (struct TWUInt256 *) rhsBytesBuffer;
    jboolean resultValue = (jboolean) TWUInt256Less(*lhsInstance, *rhsInstance);

    (*env)->ReleaseByteArrayElements(env, lhsBytesArray, lhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, lhsBytesArray);
    (*env)->DeleteLocalRef(env, lhsClass);
    (*env)->ReleaseByteArrayElements(env, rhsBytesArray, rhsBytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, rhsBytesArray);
    (*env)->DeleteLocalRef(env, rhsClass);

    return resultValue;
}

jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_isZero(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jboolean resultValue = (jboolean) TWUInt256IsZero(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jint JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_uint32Value(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jint resultValue = (jint) TWUInt256UInt32Value(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jlong JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_uint64Value(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jlong resultValue = (jlong) TWUInt256UInt64Value(*instance);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_data(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jbyteArray result = TWDataJByteArray(TWUInt256Data(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_description(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jstring result = TWStringJString(TWUInt256Description(*instance), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_format(JNIEnv *env, jobject thisObject, jint decimals) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jstring result = TWStringJString(TWUInt256Format(*instance, decimals), env);


    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jint JNICALL Java_com_wallet_crypto_trustapp_jni_UInt256_compareTo(JNIEnv *env, jobject thisObject, jobject other) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID bytesFieldID = (*env)->GetFieldID(env, thisClass, "bytes", "[B");
    jbyteArray bytesArray = (*env)->GetObjectField(env, thisObject, bytesFieldID);
    jbyte* bytesBuffer = (*env)->GetByteArrayElements(env, bytesArray, NULL);
    struct TWUInt256 *instance = (struct TWUInt256 *) bytesBuffer;

    jclass otherClass = (*env)->GetObjectClass(env, other);
    jfieldID otherBytesFieldID = (*env)->GetFieldID(env, otherClass, "bytes", "[B");
    jbyteArray otherBytesArray = (*env)->GetObjectField(env, other, otherBytesFieldID);
    jbyte* otherBytesBuffer = (*env)->GetByteArrayElements(env, otherBytesArray, NULL);
    struct TWUInt256 *otherInstance = (struct TWUInt256 *) otherBytesBuffer;
    jboolean equal = (jboolean) TWUInt256Equal(*instance, *otherInstance);
    if (equal) {
        return 0;
    }
    jboolean less = (jboolean) TWUInt256Less(*instance, *otherInstance);
    (*env)->ReleaseByteArrayElements(env, bytesArray, bytesBuffer, JNI_ABORT);
    (*env)->DeleteLocalRef(env, bytesArray);
    (*env)->DeleteLocalRef(env, thisClass);
    return less ? -1 : 1;
}

