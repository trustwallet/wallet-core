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
jobject JNICALL Java_wallet_core_jni_StoredKey_load(JNIEnv *env, jclass thisClass, jstring path);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_importPrivateKey(JNIEnv *env, jclass thisClass, jbyteArray privateKey, jstring password, jobject coin);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_importHDWallet(JNIEnv *env, jclass thisClass, jstring mnemonic, jstring password, jobject coin);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_importJSON(JNIEnv *env, jclass thisClass, jbyteArray json);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_StoredKey_identifier(JNIEnv *env, jobject thisObject);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_StoredKey_isMnemonic(JNIEnv *env, jobject thisObject);

JNIEXPORT
jsize JNICALL Java_wallet_core_jni_StoredKey_accountCount(JNIEnv *env, jobject thisObject);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_account(JNIEnv *env, jobject thisObject, jsize index);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_accountForCoin(JNIEnv *env, jobject thisObject, jobject coin, jobject wallet);

JNIEXPORT
void JNICALL Java_wallet_core_jni_StoredKey_addAccount(JNIEnv *env, jobject thisObject, jstring address, jstring derivationPath, jstring extetndedPublicKey);

JNIEXPORT
jboolean JNICALL Java_wallet_core_jni_StoredKey_store(JNIEnv *env, jobject thisObject, jstring path);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_StoredKey_decryptPrivateKey(JNIEnv *env, jobject thisObject, jstring password);

JNIEXPORT
jstring JNICALL Java_wallet_core_jni_StoredKey_decryptMnemonic(JNIEnv *env, jobject thisObject, jstring password);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_privateKey(JNIEnv *env, jobject thisObject, jobject coin, jstring password);

JNIEXPORT
jobject JNICALL Java_wallet_core_jni_StoredKey_wallet(JNIEnv *env, jobject thisObject, jstring password);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_StoredKey_exportJSON(JNIEnv *env, jobject thisObject);

JNIEXPORT
void JNICALL Java_wallet_core_jni_StoredKey_fixAddresses(JNIEnv *env, jobject thisObject, jstring password);


TW_EXTERN_C_END

#endif // JNI_TW_STOREDKEY_H
