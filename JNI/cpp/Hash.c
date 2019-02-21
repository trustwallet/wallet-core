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

#include <TrustWalletCore/TWHash.h>

#include "TWJNI.h"
#include "Hash.h"

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha1(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA1(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha256(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA256(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha512(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA512(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_keccak256(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashKeccak256(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_keccak512(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashKeccak512(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha3256(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA3_256(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha3512(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA3_512(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_ripemd(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashRIPEMD(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_blake2b(JNIEnv *env, jclass thisClass, jbyteArray data, jsize size) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashBlake2b(dataData, size), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha256RIPEMD(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA256RIPEMD(dataData), env);

    TWDataDelete(dataData);

    return result;
}

jbyteArray JNICALL Java_wallet_core_jni_Hash_sha256SHA256(JNIEnv *env, jclass thisClass, jbyteArray data) {
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    jbyteArray result = TWDataJByteArray(TWHashSHA256SHA256(dataData), env);

    TWDataDelete(dataData);

    return result;
}

