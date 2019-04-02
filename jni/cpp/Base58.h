// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_BASE58_H
#define JNI_TW_BASE58_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_Base58_encode(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_Base58_encodeNoCheck(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_Base58_decode(JNIEnv *env, jclass thisClass, jstring string);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_Base58_decodeNoCheck(JNIEnv *env, jclass thisClass, jstring string);


TW_EXTERN_C_END

#endif // JNI_TW_BASE58_H
