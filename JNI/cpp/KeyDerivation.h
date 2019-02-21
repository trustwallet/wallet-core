// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#ifndef JNI_TW_KEYDERIVATION_H
#define JNI_TW_KEYDERIVATION_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_KeyDerivation_scrypt(JNIEnv *env, jclass thisClass, jstring password, jbyteArray salt, jlong n, jint r, jint p, jsize keyLength);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_KeyDerivation_pbkdf2256(JNIEnv *env, jclass thisClass, jstring password, jbyteArray salt, jint iterations, jsize keyLength);

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_jni_KeyDerivation_pbkdf2512(JNIEnv *env, jclass thisClass, jstring password, jbyteArray salt, jint iterations, jsize keyLength);


TW_EXTERN_C_END

#endif // JNI_TW_KEYDERIVATION_H
