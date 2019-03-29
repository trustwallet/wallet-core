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

#include <TrustWalletCore/TWOntologySigner.h>

#include "TWJNI.h"
#include "OntologySigner.h"

jobject JNICALL Java_wallet_core_jni_OntologySigner_sign(JNIEnv *env, jclass thisClass, jobject data) {
    jclass dataClass = (*env)->GetObjectClass(env, data);
    jmethodID dataToByteArrayMethodID = (*env)->GetMethodID(env, dataClass, "toByteArray", "()[B");
    jbyteArray dataByteArray = (*env)->CallObjectMethod(env, data, dataToByteArrayMethodID);
    TWData *dataData = TWDataCreateWithJByteArray(env, dataByteArray);
    jbyteArray resultData = TWDataJByteArray(TWOntologySignerSign(dataData), env);
    jclass resultClass = (*env)->FindClass(env, "wallet/core/jni/proto/Ontology$SigningOutput");
    jmethodID parseFromMethodID = (*env)->GetStaticMethodID(env, resultClass, "parseFrom", "([B)Lwallet/core/jni/proto/Ontology$SigningOutput;");
    jobject result = (*env)->CallStaticObjectMethod(env, resultClass, parseFromMethodID, resultData);

    (*env)->DeleteLocalRef(env, resultClass);
    (*env)->DeleteLocalRef(env, dataByteArray);
    (*env)->DeleteLocalRef(env, dataClass);

    return result;
}

