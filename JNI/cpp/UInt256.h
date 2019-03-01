// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_UINT256_H
#define JNI_TW_UINT256_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_UInt256_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_UInt256_initWithString(JNIEnv *env, jclass thisClass, jstring string);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_UInt256_initWithUInt32(JNIEnv *env, jclass thisClass, jint value);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_UInt256_initWithUInt64(JNIEnv *env, jclass thisClass, jlong value);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_UInt256_zero(JNIEnv *env, jclass thisClass);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_UInt256_one(JNIEnv *env, jclass thisClass);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_UInt256_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_UInt256_less(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_UInt256_isZero(JNIEnv *env, jobject thisObject);

JNIEXPORT
jint JNICALL Java_wallet_core_jni_UInt256_uint32Value(JNIEnv *env, jobject thisObject);

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_UInt256_uint64Value(JNIEnv *env, jobject thisObject);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_UInt256_data(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_UInt256_description(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_UInt256_format(JNIEnv *env, jobject thisObject, jint decimals);

JNIEXPORT
jint JNICALL Java_wallet_core_jni_UInt256_compareTo(JNIEnv *env, jobject thisObject, jobject other);


TW_EXTERN_C_END

#endif // JNI_TW_UINT256_H
