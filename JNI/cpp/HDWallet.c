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

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWPublicKey.h>

#include "TWJNI.h"
#include "HDWallet.h"

jlong JNICALL Java_wallet_core_jni_HDWallet_nativeCreate(JNIEnv *env, jclass thisClass, jint strength, jstring passphrase) {
    TWString *passphraseString = TWStringCreateWithJString(env, passphrase);
    struct TWHDWallet *instance = TWHDWalletCreate(strength, passphraseString);
    TWStringDelete(passphraseString);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_HDWallet_nativeCreateWithMnemonic(JNIEnv *env, jclass thisClass, jstring mnemonic, jstring passphrase) {
    TWString *mnemonicString = TWStringCreateWithJString(env, mnemonic);
    TWString *passphraseString = TWStringCreateWithJString(env, passphrase);
    struct TWHDWallet *instance = TWHDWalletCreateWithMnemonic(mnemonicString, passphraseString);
    TWStringDelete(mnemonicString);
    TWStringDelete(passphraseString);
    return (jlong) instance;
}

jlong JNICALL Java_wallet_core_jni_HDWallet_nativeCreateWithData(JNIEnv *env, jclass thisClass, jbyteArray data, jstring passphrase) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    TWString *passphraseString = TWStringCreateWithJString(env, passphrase);
    struct TWHDWallet *instance = TWHDWalletCreateWithData(dataData, passphraseString);
    TWDataDelete(dataData);
    TWStringDelete(passphraseString);
    return (jlong) instance;
}

void JNICALL Java_wallet_core_jni_HDWallet_nativeDelete(JNIEnv *env, jclass thisClass, jlong handle) {
    TWHDWalletDelete((struct TWHDWallet *) handle);
}

jboolean JNICALL Java_wallet_core_jni_HDWallet_isValid(JNIEnv *env, jclass thisClass, jstring mnemonic) {
    TWString *mnemonicString = TWStringCreateWithJString(env, mnemonic);
    jboolean resultValue = (jboolean) TWHDWalletIsValid(mnemonicString);

    TWStringDelete(mnemonicString);

    return resultValue;
}

jobject JNICALL Java_wallet_core_jni_HDWallet_getPublicKeyFromExtended(JNIEnv *env, jclass thisClass, jstring extended, jobject versionPublic, jobject versionPrivate, jint change, jint address) {
    TWString *extendedString = TWStringCreateWithJString(env, extended);
    jclass versionPublicClass = (*env)->GetObjectClass(env, versionPublic);
    jmethodID versionPublicValueMethodID = (*env)->GetMethodID(env, versionPublicClass, "value", "()I");
    jint versionPublicValue = (*env)->CallIntMethod(env, versionPublic, versionPublicValueMethodID);
    jclass versionPrivateClass = (*env)->GetObjectClass(env, versionPrivate);
    jmethodID versionPrivateValueMethodID = (*env)->GetMethodID(env, versionPrivateClass, "value", "()I");
    jint versionPrivateValue = (*env)->CallIntMethod(env, versionPrivate, versionPrivateValueMethodID);
    struct TWPublicKey result = TWHDWalletGetPublicKeyFromExtended(extendedString, versionPublicValue, versionPrivateValue, change, address);

    TWStringDelete(extendedString);
    (*env)->DeleteLocalRef(env, versionPublicClass);
    (*env)->DeleteLocalRef(env, versionPrivateClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PublicKey");
    jbyteArray resultArray = (*env)->NewByteArray(env, sizeof(struct TWPublicKey));
    (*env)->SetByteArrayRegion(env, resultArray, 0, sizeof(struct TWPublicKey), (jbyte *) &result);
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "([B)Lwallet/core/jni/PublicKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, resultArray);
}

jstring JNICALL Java_wallet_core_jni_HDWallet_getAddressFromExtended(JNIEnv *env, jclass thisClass, jstring extended, jobject coinType, jint change, jint address) {
    TWString *extendedString = TWStringCreateWithJString(env, extended);
    jclass coinTypeClass = (*env)->GetObjectClass(env, coinType);
    jmethodID coinTypeValueMethodID = (*env)->GetMethodID(env, coinTypeClass, "value", "()I");
    jint coinTypeValue = (*env)->CallIntMethod(env, coinType, coinTypeValueMethodID);
    jstring result = TWStringJString(TWHDWalletGetAddressFromExtended(extendedString, coinTypeValue, change, address), env);

    TWStringDelete(extendedString);
    (*env)->DeleteLocalRef(env, coinTypeClass);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_HDWallet_seed(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWHDWallet *instance = (struct TWHDWallet *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jbyteArray result = TWDataJByteArray(TWHDWalletSeed(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_HDWallet_mnemonic(JNIEnv *env, jobject thisObject) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWHDWallet *instance = (struct TWHDWallet *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jstring result = TWStringJString(TWHDWalletMnemonic(instance), env);


    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jobject JNICALL Java_wallet_core_jni_HDWallet_getKey(JNIEnv *env, jobject thisObject, jobject purpose, jobject coin, jint account, jint change, jint address) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWHDWallet *instance = (struct TWHDWallet *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jclass purposeClass = (*env)->GetObjectClass(env, purpose);
    jmethodID purposeValueMethodID = (*env)->GetMethodID(env, purposeClass, "value", "()I");
    jint purposeValue = (*env)->CallIntMethod(env, purpose, purposeValueMethodID);
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    struct TWPrivateKey *result = TWHDWalletGetKey(instance, purposeValue, coinValue, account, change, address);

    (*env)->DeleteLocalRef(env, purposeClass);
    (*env)->DeleteLocalRef(env, coinClass);

    (*env)->DeleteLocalRef(env, thisClass);

    jclass class = (*env)->FindClass(env, "wallet/core/jni/PrivateKey");
    if (result == NULL) {
        return NULL;
    }
    jmethodID method = (*env)->GetStaticMethodID(env, class, "createFromNative", "(J)Lwallet/core/jni/PrivateKey;");
    return (*env)->CallStaticObjectMethod(env, class, method, (jlong) result);
}

jstring JNICALL Java_wallet_core_jni_HDWallet_getExtendedPrivateKey(JNIEnv *env, jobject thisObject, jobject purpose, jobject coin, jobject version) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWHDWallet *instance = (struct TWHDWallet *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jclass purposeClass = (*env)->GetObjectClass(env, purpose);
    jmethodID purposeValueMethodID = (*env)->GetMethodID(env, purposeClass, "value", "()I");
    jint purposeValue = (*env)->CallIntMethod(env, purpose, purposeValueMethodID);
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    jclass versionClass = (*env)->GetObjectClass(env, version);
    jmethodID versionValueMethodID = (*env)->GetMethodID(env, versionClass, "value", "()I");
    jint versionValue = (*env)->CallIntMethod(env, version, versionValueMethodID);
    jstring result = TWStringJString(TWHDWalletGetExtendedPrivateKey(instance, purposeValue, coinValue, versionValue), env);

    (*env)->DeleteLocalRef(env, purposeClass);
    (*env)->DeleteLocalRef(env, coinClass);
    (*env)->DeleteLocalRef(env, versionClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

jstring JNICALL Java_wallet_core_jni_HDWallet_getExtendedPublicKey(JNIEnv *env, jobject thisObject, jobject purpose, jobject coin, jobject version) {
    jclass thisClass = (*env)->GetObjectClass(env, thisObject);
    jfieldID handleFieldID = (*env)->GetFieldID(env, thisClass, "nativeHandle", "J");
    struct TWHDWallet *instance = (struct TWHDWallet *) (*env)->GetLongField(env, thisObject, handleFieldID);

    jclass purposeClass = (*env)->GetObjectClass(env, purpose);
    jmethodID purposeValueMethodID = (*env)->GetMethodID(env, purposeClass, "value", "()I");
    jint purposeValue = (*env)->CallIntMethod(env, purpose, purposeValueMethodID);
    jclass coinClass = (*env)->GetObjectClass(env, coin);
    jmethodID coinValueMethodID = (*env)->GetMethodID(env, coinClass, "value", "()I");
    jint coinValue = (*env)->CallIntMethod(env, coin, coinValueMethodID);
    jclass versionClass = (*env)->GetObjectClass(env, version);
    jmethodID versionValueMethodID = (*env)->GetMethodID(env, versionClass, "value", "()I");
    jint versionValue = (*env)->CallIntMethod(env, version, versionValueMethodID);
    jstring result = TWStringJString(TWHDWalletGetExtendedPublicKey(instance, purposeValue, coinValue, versionValue), env);

    (*env)->DeleteLocalRef(env, purposeClass);
    (*env)->DeleteLocalRef(env, coinClass);
    (*env)->DeleteLocalRef(env, versionClass);

    (*env)->DeleteLocalRef(env, thisClass);

    return result;
}

