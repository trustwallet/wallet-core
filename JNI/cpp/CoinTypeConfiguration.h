// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_COINTYPECONFIGURATION_H
#define JNI_TW_COINTYPECONFIGURATION_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinTypeConfiguration_getSymbol(JNIEnv *env, jclass thisClass, jobject type);

JNIEXPORT
jint JNICALL Java_wallet_core_jni_CoinTypeConfiguration_getDecimals(JNIEnv *env, jclass thisClass, jobject type);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinTypeConfiguration_getTransactionURL(JNIEnv *env, jclass thisClass, jobject type, jstring transactionID);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinTypeConfiguration_getID(JNIEnv *env, jclass thisClass, jobject type);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_CoinTypeConfiguration_getName(JNIEnv *env, jclass thisClass, jobject type);


TW_EXTERN_C_END

#endif // JNI_TW_COINTYPECONFIGURATION_H
