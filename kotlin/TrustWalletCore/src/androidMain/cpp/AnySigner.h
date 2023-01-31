// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef JNI_TW_ANYSIGNER_H
#define JNI_TW_ANYSIGNER_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_com_trustwallet_core_AnySigner_nativeSign(JNIEnv *env, jobject thisClass, jbyteArray input, uint32_t coin);

JNIEXPORT
jboolean JNICALL Java_com_trustwallet_core_AnySigner_supportsJSON(JNIEnv *env, jobject thisClass, uint32_t coin);

JNIEXPORT
jstring JNICALL Java_com_trustwallet_core_AnySigner_signJSON(JNIEnv *env, jobject thisClass, jstring json, jbyteArray key, uint32_t coin);

JNIEXPORT
jbyteArray JNICALL Java_com_trustwallet_core_AnySigner_nativePlan(JNIEnv *env, jobject thisClass, jbyteArray input, uint32_t coin);

TW_EXTERN_C_END

#endif // JNI_TW_ANYSIGNER_H
