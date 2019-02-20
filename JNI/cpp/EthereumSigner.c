// Copyright © 2017-2019 Trust.
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

#include <TrustWalletCore/TWEthereumSigner.h>

#include "TWJNI.h"
#include "EthereumSigner.h"

jobject JNICALL Java_com_wallet_crypto_trustapp_jni_EthereumSigner_sign(JNIEnv *env, jclass thisClass, jobject input) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    jbyteArray resultData = TWDataJByteArray(TWEthereumSignerSign(inputData), env);
    jclass resultClass = (*env)->FindClass(env, "com/wallet/crypto/trustapp/proto/Proto$SigningOutput");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lcom/wallet/crypto/trustapp/proto/Proto$SigningOutput;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);

    return result;
}

