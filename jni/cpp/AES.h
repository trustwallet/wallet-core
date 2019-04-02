// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_AES_H
#define JNI_TW_AES_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_AES_cbcencrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_AES_cbcdecrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_AES_ctrencrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_AES_ctrdecrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv);


TW_EXTERN_C_END

#endif // JNI_TW_AES_H
