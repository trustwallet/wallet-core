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

#include <TrustWalletCore/TWAccount.h>

#include "TWJNI.h"
#include "Account.h"

jlong JNICALL Java_wallet_core_jni_Account_nativeCreate(JNIEnv *env, jclass thisClass, jstring address, jstring derivationPath, jstring extendedPublicKey) {
    TWString *addressString = TWStringCreateWithJString(env, address);
    TWString *derivationPathString = TWStringCreateWithJString(env, derivationPath);
    TWString *extendedPublicKeyString = TWStringCreateWithJString(env, extendedPublicKey);
    struct TWAccount *instance = TWAccountCreate(addressString, derivationPathString, extendedPublicKeyString);
    TWStringDelete(addressString);
    TWStringDelete(derivationPathString);
    TWStringDelete(extendedPublicKeyString);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_Account_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWAccountDelete((struct TWAccount *) handle);
}

jstring JNICALL Java_wallet_core_jni_Account_address(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWAccount *instance = (struct TWAccount *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWAccountAddress(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_Account_derivationPath(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWAccount *instance = (struct TWAccount *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWAccountDerivationPath(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_Account_extendedPublicKey(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWAccount *instance = (struct TWAccount *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWAccountExtendedPublicKey(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_Account_coin(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWAccount *instance = (struct TWAccount *) (*env)->GetLongField(env, thisObject, handleFieldID);

    enum TWCoinType result = TWAccountCoin(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/CoinType");
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromValue", "(I)Lwallet/core/jni/CoinType;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jint) result);
}

