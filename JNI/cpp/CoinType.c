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

#include <TrustWalletCore/TWCoinType.h>

#include "TWJNI.h"
#include "CoinType.h"

jobject JNICALL Java_wallet_core_jni_CoinType_purpose(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "value", "I");
    enum TWCoinType instance = (enum TWCoinType) (*env)->GetIntField(env, thisObject, handleFieldID);

    enum TWPurpose result = TWCoinTypePurpose(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/Purpose");
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromValue", "(I)Lwallet/core/jni/Purpose;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jint) result);
}

jobject JNICALL Java_wallet_core_jni_CoinType_curve(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "value", "I");
    enum TWCoinType instance = (enum TWCoinType) (*env)->GetIntField(env, thisObject, handleFieldID);

    enum TWCurve result = TWCoinTypeCurve(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/Curve");
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromValue", "(I)Lwallet/core/jni/Curve;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jint) result);
}

jboolean JNICALL Java_wallet_core_jni_CoinType_validate(JNIEnv *env, jobject thisObject, jstring address) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "value", "I");
    enum TWCoinType instance = (enum TWCoinType) (*env)->GetIntField(env, thisObject, handleFieldID);

    TWString *addressString = TWStringCreateWithJString(env, address);
    jboolean resultValue = (jboolean) TWCoinTypeValidate(instance, addressString);

    TWStringDelete(addressString);

    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

