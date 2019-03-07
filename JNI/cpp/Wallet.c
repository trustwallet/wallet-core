// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

#include <jni.h>
#include <stdio.h>
#include <string.h>

#include <TrustWalletCore/TWWallet.h>

#include "TWJNI.h"
#include "Wallet.h"

jboolean JNICALL Java_wallet_core_jni_Wallet_validate(JNIEnv *env, jclass thisClass, jstring string, jobject coin) {
    TWString *stringString = TWStringCreateWithJString(env, string);
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    jboolean resultValue = (jboolean) TWWalletValidate(stringString, coinValue);

    TWStringDelete(stringString);
    (*env)->DeleteLocalRef(env, coinClass);

    return resultValue;
}

