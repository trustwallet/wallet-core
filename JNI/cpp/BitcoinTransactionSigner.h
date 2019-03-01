// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_BITCOINTRANSACTIONSIGNER_H
#define JNI_TW_BITCOINTRANSACTIONSIGNER_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_nativeCreate(JNIEnv *env, jclass thisClass, jobject input);

JNIEXPORT
void JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_plan(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_BitcoinTransactionSigner_sign(JNIEnv *env, jobject thisObject);


TW_EXTERN_C_END

#endif // JNI_TW_BITCOINTRANSACTIONSIGNER_H
