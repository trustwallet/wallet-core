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

jobject JNICALL Java_wallet_core_jni_StoredKey_load(JNIEnv *env, jclass thisClass, jstring path) {
    TWString *pathString = TWStringCreateWithJString(env, path);
    struct TWStoredKey *result = TWStoredKeyLoad(pathString);

    TWStringDelete(pathString);

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

jobject JNICALL Java_wallet_core_jni_StoredKey_importHDWallet(JNIEnv *env, jclass thisClass, jstring mnemonic, jstring password, jobject coin) {
    TWString *mnemonicString = TWStringCreateWithJString(env, mnemonic);
    TWString *passwordString = TWStringCreateWithJString(env, password);
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    struct TWStoredKey *result = TWStoredKeyImportHDWallet(mnemonicString, passwordString, coinValue);

    TWStringDelete(mnemonicString);
    TWStringDelete(passwordString);
    (*env)->DeleteLocalRef(env, coinClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/StoredKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/StoredKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jobject JNICALL Java_wallet_core_jni_StoredKey_importJSON(JNIEnv *env, jclass thisClass, jbyteArray json) {
    TWData *jsonData = TWDataCreateWithJByteArray(env, json);
    struct TWStoredKey *result = TWStoredKeyImportJSON(jsonData);

    TWDataDelete(jsonData);

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

jboolean JNICALL Java_wallet_core_jni_StoredKey_isMnemonic(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jboolean resultValue = (jboolean) TWStoredKeyIsMnemonic(instance);


    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
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

jobject JNICALL Java_wallet_core_jni_StoredKey_accountForCoin(JNIEnv *env, jobject thisObject, jobject coin, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    TWString *passwordString = TWStringCreateWithJString(env, password);
    struct TWAccount *result = TWStoredKeyAccountForCoin(instance, coinValue, passwordString);

    (*env)->DeleteLocalRef(env, coinClass);
    TWStringDelete(passwordString);

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

jboolean JNICALL Java_wallet_core_jni_StoredKey_store(JNIEnv *env, jobject thisObject, jstring path) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *pathString = TWStringCreateWithJString(env, path);
    jboolean resultValue = (jboolean) TWStoredKeyStore(instance, pathString);

    TWStringDelete(pathString);

    (*env)->DeleteLocalRef(env, thisClass);

    return resultValue;
}

jbyteArray JNICALL Java_wallet_core_jni_StoredKey_decryptPrivateKey(JNIEnv *env, jobject thisObject, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *passwordString = TWStringCreateWithJString(env, password);
    jbyteArray result = TWDataJByteArray(TWStoredKeyDecryptPrivateKey(instance, passwordString), env);

    TWStringDelete(passwordString);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_StoredKey_decryptMnemonic(JNIEnv *env, jobject thisObject, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *passwordString = TWStringCreateWithJString(env, password);
    jstring result = TWStringJString(TWStoredKeyDecryptMnemonic(instance, passwordString), env);

    TWStringDelete(passwordString);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_StoredKey_wallet(JNIEnv *env, jobject thisObject, jstring password) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    TWString *passwordString = TWStringCreateWithJString(env, password);
    struct TWHDWallet *result = TWStoredKeyWallet(instance, passwordString);

    TWStringDelete(passwordString);

    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/HDWallet");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/HDWallet;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jbyteArray JNICALL Java_wallet_core_jni_StoredKey_exportJSON(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWStoredKey *instance = (struct TWStoredKey *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWStoredKeyExportJSON(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

