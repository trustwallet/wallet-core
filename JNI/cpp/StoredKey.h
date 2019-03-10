// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_STOREDKEY_H
#define JNI_TW_STOREDKEY_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jlong JNICALL Java_wallet_core_jni_StoredKey_nativeCreate(JNIEnv *env, jclass thisClass, jstring password);

JNIEXPORT
void JNICALL Java_wallet_core_jni_StoredKey_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_load(JNIEnv *env, jclass thisClass, jstring path, jstring password);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_importPrivateKey(JNIEnv *env, jclass thisClass, jbyteArray privateKey, jstring password, jobject coin);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_importHDWallet(JNIEnv *env, jclass thisClass, jstring mnemonic, jstring password, jstring derivationPath);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_StoredKey_identifier(JNIEnv *env, jobject thisObject);

JNIEXPORT
jsize JNICALL Java_wallet_core_jni_StoredKey_accountCount(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_account(JNIEnv *env, jobject thisObject, jsize index);

JNIEXPORT
void JNICALL Java_wallet_core_jni_StoredKey_addAccount(JNIEnv *env, jobject thisObject, jstring address, jstring derivationPath, jstring extetndedPublicKey);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_StoredKey_store(JNIEnv *env, jobject thisObject, jstring path, jstring password);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_StoredKey_exportPrivateKey(JNIEnv *env, jobject thisObject, jstring password);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_StoredKey_exportMnemonic(JNIEnv *env, jobject thisObject, jstring password);


TW_EXTERN_C_END

#endif // JNI_TW_STOREDKEY_H
