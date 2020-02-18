// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#ifndef JNI_TW_UTXOPLANNER_H
#define JNI_TW_UTXOPLANNER_H

#include <jni.h>
#include <TrustWalletCore/TWBase.h>

TW_EXTERN_C_BEGIN

JNIEXPORT
jbyteArray JNICALL Java_wallet_core_java_UTXOPlanner_nativePlan(JNIEnv *env, jclass thisClass, jbyteArray input, jint coin);

TW_EXTERN_C_END

#endif // JNI_TW_UTXOPLANNER_H
