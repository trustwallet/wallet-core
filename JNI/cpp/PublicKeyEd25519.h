// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_PUBLICKEYED25519_H
#define JNI_TW_PUBLICKEYED25519_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_PublicKeyEd25519_initWithData(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_PublicKeyEd25519_isValid(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_PublicKeyEd25519_data(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_PublicKeyEd25519_description(JNIEnv *env, jobject thisObject);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_PublicKeyEd25519_verify(JNIEnv *env, jobject thisObject, jbyteArray signature, jbyteArray message);


TW_EXTERN_C_END

#endif // JNI_TW_PUBLICKEYED25519_H
