// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_COINTYPE_H
#define JNI_TW_COINTYPE_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_CoinType_purpose(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_CoinType_curve(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_CoinType_xpubVersion(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_CoinType_xprvVersion(JNIEnv *env, jobject thisObject);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_CoinType_validate(JNIEnv *env, jobject thisObject, jstring address);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinType_derivationPath(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinType_deriveAddress(JNIEnv *env, jobject thisObject, jobject privateKey);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinType_deriveAddressFromPublicKey(JNIEnv *env, jobject thisObject, jobject publicKey);


TW_EXTERN_C_END

#endif // JNI_TW_COINTYPE_H
