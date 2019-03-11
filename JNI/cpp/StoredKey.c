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

#include <TrustWalletCore/TWStoredKey.h>

#include "TWJNI.h"
#include "StoredKey.h"

jlong JNICALL Java_wallet_core_jni_StoredKey_nativeCreate(JNIEnv *env, jclass thisClass, jstring password) {
    TWString *passwordString = TWStringCreateWithJString(env, password);
    struct TWStoredKey *instance = TWStoredKeyCreate(passwordString);
    TWStringDelete(passwordString);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_StoredKey_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWStoredKeyDelete((struct TWStoredKey *) handle);
}

jobject JNICALL Java_wallet_core_jni_StoredKey_load(JNIEnv *env, jclass thisClass, jstring path, jstring password) {
    TWString *pathString = TWStringCreateWithJString(env, path);
    TWString *passwordString = TWStringCreateWithJString(env, password);
    struct TWStoredKey *result = TWStoredKeyLoad(pathString, passwordString);

    TWStringDelete(pathString);
    TWStringDelete(passwordString);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/StoredKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/StoredKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jobject JNICALL Java_wallet_core_jni_StoredKey_importPrivateKey(JNIEnv *env, jclass thisClass, jbyteArray privateKey, jstring password, jobject coin) {
    TWData *privateKeyData = TWDataCreateWithJByteArray(env, privateKey);
    TWString *passwordString = TWStringCreateWithJString(env, password);
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    struct TWStoredKey *result = TWStoredKeyImportPrivateKey(privateKeyData, passwordString, coinValue);

    TWDataDelete(privateKeyData);
    TWStringDelete(passwordString);
    (*env)->DeleteLocalRef(env, coinClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/StoredKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/StoredKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jobject JNICALL Java_wallet_core_jni_StoredKey_importHDWallet(JNIEnv *env, jclass thisClass, jstring mnemonic, jstring password, jstring derivationPath) {
    TWString *mnemonicString = TWStringCreateWithJString(env, mnemonic);
    TWString *passwordString = TWStringCreateWithJString(env, password);
    TWString *derivationPathString = TWStringCreateWithJString(env, derivationPath);
    struct TWStoredKey *result = TWStoredKeyImportHDWallet(mnemonicString, passwordString, derivationPathString);

    TWStringDelete(mnemonicString);
    TWStringDelete(passwordString);
    TWStringDelete(derivationPathString);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/StoredKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/StoredKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jstring JNICALL Java_wallet_core_jni_StoredKey_identifier(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWStoredKeyIdentifier(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jsize JNICALL Java_wallet_core_jni_StoredKey_accountCount(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jsize resultValue = (jsize) TWStoredKeyAccountCount(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jobject JNICALL Java_wallet_core_jni_StoredKey_account(JNIEnv *env, jobject thisObject, jsize index) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    struct TWAccount *result = TWStoredKeyAccount(instance, index);


    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/Account");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/Account;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

void JNICALL Java_wallet_core_jni_StoredKey_addAccount(JNIEnv *env, jobject thisObject, jstring address, jstring derivationPath, jstring extetndedPublicKey) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *addressString = TWStringCreateWithJString(env, address);
    TWString *derivationPathString = TWStringCreateWithJString(env, derivationPath);
    TWString *extetndedPublicKeyString = TWStringCreateWithJString(env, extetndedPublicKey);
    TWStoredKeyAddAccount(instance, addressString, derivationPathString, extetndedPublicKeyString);

    TWStringDelete(addressString);
    TWStringDelete(derivationPathString);
    TWStringDelete(extetndedPublicKeyString);

    (*env)->DeleteLocalRef(env, thisClass);
}

jboolean JNICALL Java_wallet_core_jni_StoredKey_store(JNIEnv *env, jobject thisObject, jstring path, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *pathString = TWStringCreateWithJString(env, path);
    TWString *passwordString = TWStringCreateWithJString(env, password);
    jboolean resultValue = (jboolean) TWStoredKeyStore(instance, pathString, passwordString);

    TWStringDelete(pathString);
    TWStringDelete(passwordString);

    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_StoredKey_exportPrivateKey(JNIEnv *env, jobject thisObject, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *passwordString = TWStringCreateWithJString(env, password);
    jbyteArray result = TWDataJByteArray(TWStoredKeyExportPrivateKey(instance, passwordString), env);

    TWStringDelete(passwordString);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_StoredKey_exportMnemonic(JNIEnv *env, jobject thisObject, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *passwordString = TWStringCreateWithJString(env, password);
    jstring result = TWStringJString(TWStoredKeyExportMnemonic(instance, passwordString), env);

    TWStringDelete(passwordString);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

