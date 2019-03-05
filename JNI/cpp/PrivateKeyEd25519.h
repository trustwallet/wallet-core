// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_PRIVATEKEYED25519_H
#define JNI_TW_PRIVATEKEYED25519_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeCreate(JNIEnv *env, jclass thisClass);

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeCreateCopy(JNIEnv *env, jclass thisClass, jobject key);

JNIEXPORT
void JNICALL Java_wallet_core_jni_PrivateKeyEd25519_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_PrivateKeyEd25519_isValid(JNIEnv *env, jclass thisClass, jbyteArray data);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_PrivateKeyEd25519_data(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_PrivateKeyEd25519_getPublicKey(JNIEnv *env, jobject thisObject);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_PrivateKeyEd25519_sign(JNIEnv *env, jobject thisObject, jbyteArray digest);


TW_EXTERN_C_END

#endif // JNI_TW_PRIVATEKEYED25519_H
