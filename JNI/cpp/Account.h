// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_ACCOUNT_H
#define JNI_TW_ACCOUNT_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_Account_nativeCreate(JNIEnv *env, jclass thisClass, jstring address, jstring derivationPath, jstring extendedPublicKey);

JNIEXPORT
void JNICALL Java_wallet_core_jni_Account_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_Account_address(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_Account_derivationPath(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_Account_extendedPublicKey(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_Account_coin(JNIEnv *env, jobject thisObject);


TW_EXTERN_C_END

#endif // JNI_TW_ACCOUNT_H
