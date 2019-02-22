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

#include <TrustWalletCore/TWWanchainSigner.h>

#include "TWJNI.h"
#include "WanchainSigner.h"

jobject JNICALL Java_wallet_core_jni_WanchainSigner_sign(JNIEnv *env, jclass thisClass, jobject input) {
    jclass inputClass = (*env)->GetObjectClass(env, input);
    jmethodID inputToByteArrayMethodID = (*env)->GetMethodID(env, inputClass, "toByteArray", "()[B");
    jbyteArray inputByteArray = (*env)->CallObjectMethod(env, input, inputToByteArrayMethodID);
    TWData *inputData = TWDataCreateWithJByteArray(env, inputByteArray);
    jbyteArray resultData = TWDataJByteArray(TWWanchainSignerSign(inputData), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Proto$SigningOutput");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Proto$SigningOutput;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);
    (*env)->DeleteLocalRef(env, inputByteArray);
    (*env)->DeleteLocalRef(env, inputClass);

    return result;
}

