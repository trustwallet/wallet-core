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

#include <TrustWalletCore/TWAES.h>

#include "TWJNI.h"
#include "AES.h"

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_AES_cbcencrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv) {
    TWData *keyData = TWDataCreateWithJByteArray(env, key);
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    TWData *ivData = TWDataCreateWithJByteArray(env, iv);
    jbyteArray result = TWDataJByteArray(TWAESCBCEncrypt(keyData, dataData, ivData), env);

    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);

    return result;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_AES_cbcdecrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv) {
    TWData *keyData = TWDataCreateWithJByteArray(env, key);
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    TWData *ivData = TWDataCreateWithJByteArray(env, iv);
    jbyteArray result = TWDataJByteArray(TWAESCBCDecrypt(keyData, dataData, ivData), env);

    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);

    return result;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_AES_ctrencrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv) {
    TWData *keyData = TWDataCreateWithJByteArray(env, key);
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    TWData *ivData = TWDataCreateWithJByteArray(env, iv);
    jbyteArray result = TWDataJByteArray(TWAESCTREncrypt(keyData, dataData, ivData), env);

    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);

    return result;
}

jbyteArray JNICALL Java_com_wallet_crypto_trustapp_jni_AES_ctrdecrypt(JNIEnv *env, jclass thisClass, jbyteArray key, jbyteArray data, jbyteArray iv) {
    TWData *keyData = TWDataCreateWithJByteArray(env, key);
    TWData *dataData = TWDataCreateWithJByteArray(env, data);
    TWData *ivData = TWDataCreateWithJByteArray(env, iv);
    jbyteArray result = TWDataJByteArray(TWAESCTRDecrypt(keyData, dataData, ivData), env);

    TWDataDelete(keyData);
    TWDataDelete(dataData);
    TWDataDelete(ivData);

    return result;
}

