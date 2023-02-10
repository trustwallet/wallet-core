// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <jni.h>
#include <string.h>

static JavaVM* cachedJVM;

extern "C" {
    uint32_t random32();
    void random_buffer(uint8_t *buf, size_t len);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    cachedJVM = jvm;
    return JNI_VERSION_1_2;
}

uint32_t random32() {
    uint32_t result;
    random_buffer((uint8_t*) &result, sizeof(uint32_t));
    return result;
}

void random_buffer(uint8_t *buf, size_t len) {
    JNIEnv *env;
    cachedJVM->AttachCurrentThread(&env, nullptr);

    // SecureRandom random = new SecureRandom();
    jclass secureRandomClass = env->FindClass("java/security/SecureRandom");
    jmethodID constructor = env->GetMethodID(secureRandomClass, "<init>", "()V");
    jobject random = env->NewObject(secureRandomClass, constructor);

    //byte array[] = new byte[len];
    jbyteArray array = env->NewByteArray(static_cast<jsize>(len));

    //random.nextBytes(bytes);
    jmethodID nextBytes = env->GetMethodID(secureRandomClass, "nextBytes", "([B)V");
    env->CallVoidMethod(random, nextBytes, array);

    jbyte* bytes = env->GetByteArrayElements(array, nullptr);
    memcpy(buf, bytes, len);
    env->ReleaseByteArrayElements(array, bytes, JNI_ABORT);

    env->DeleteLocalRef(array);
    env->DeleteLocalRef(random);
    env->DeleteLocalRef(secureRandomClass);
}
