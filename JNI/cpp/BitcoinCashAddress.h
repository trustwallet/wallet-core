// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_BITCOINCASHADDRESS_H
#define JNI_TW_BITCOINCASHADDRESS_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_initWithString(JNIEnv *env, jclass thisClass, jstring string);

JNIEXPORT
jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_initWithPublicKey(JNIEnv *env, jclass thisClass, jobject publicKey);

JNIEXPORT
jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_equals(JNIEnv *env, jclass thisClass, jobject lhs, jobject rhs);

JNIEXPORT
jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_isValid(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jboolean JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_isValidString(JNIEnv *env, jclass thisClass, jstring string);

JNIEXPORT
jstring JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_description(JNIEnv *env, jobject thisObject);

JNIEXPORT
jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_data(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_com_wallet_crypto_trustapp_jni_BitcoinCashAddress_legacyAddress(JNIEnv *env, jobject thisObject);


TW_EXTERN_C_END

#endif // JNI_TW_BITCOINCASHADDRESS_H
