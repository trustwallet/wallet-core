// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_HDWALLET_H
#define JNI_TW_HDWALLET_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_HDWallet_nativeCreate(JNIEnv *env, jclass thisClass, jint strength, jstring passphrase);

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_HDWallet_nativeCreateWithMnemonic(JNIEnv *env, jclass thisClass, jstring mnemonic, jstring passphrase);

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_HDWallet_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data, jstring passphrase);

JNIEXPORT
void JNICALL Java_wallet_core_jni_HDWallet_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_HDWallet_isValid(JNIEnv *env, jclass thisClass, jstring mnemonic);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_HDWallet_getPublicKeyFromExtended(JNIEnv *env, jclass thisClass, jstring extended, jobject curve, jobject versionPublic, jobject versionPrivate, jint change, jint address);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_HDWallet_getAddressFromExtended(JNIEnv *env, jclass thisClass, jstring extended, jobject curve, jobject coinType, jint change, jint address);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_HDWallet_seed(JNIEnv *env, jobject thisObject);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_HDWallet_mnemonic(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_HDWallet_getKey(JNIEnv *env, jobject thisObject, jobject curve, jobject purpose, jobject coin, jint account, jint change, jint address);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_HDWallet_getExtendedPrivateKey(JNIEnv *env, jobject thisObject, jobject curve, jobject purpose, jobject coin, jobject version);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_HDWallet_getExtendedPublicKey(JNIEnv *env, jobject thisObject, jobject curve, jobject purpose, jobject coin, jobject version);


TW_EXTERN_C_END

#endif // JNI_TW_HDWALLET_H
