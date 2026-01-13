// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <jni.h>
#include <string.h>
#include <stdint.h>
#include <fstream>
#include <vector>

static JavaVM* cachedJVM = nullptr;

extern "C" {
    int random_buffer(uint8_t *buf, size_t len);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, [[maybe_unused]] void *reserved) {
    cachedJVM = jvm;
    return JNI_VERSION_1_6;
}

class JNIEnvContext {
public:
    explicit JNIEnvContext() {
        jint attachResult = cachedJVM->GetEnv(reinterpret_cast<void**>(&_env), JNI_VERSION_1_6);
        if (attachResult == JNI_EDETACHED) {
            attachResult = cachedJVM->AttachCurrentThread(reinterpret_cast<void**>(&_env), nullptr);
            _needsDetach = true;
        }

        if (attachResult != JNI_OK || _env == nullptr) {
            _env = nullptr;
            _needsDetach = false;
        }
    }

    ~JNIEnvContext() {
        if (_env != nullptr) {
            for (size_t i = 0; i < _refs.size(); i++) {
                if (_refs[i] != nullptr) {
                    _env->DeleteLocalRef(_refs[i]);
                }
            }
        }

        if (_needsDetach) {
            cachedJVM->DetachCurrentThread();
        }
    }

    bool ok() {
        return _env != nullptr;
    }

    JNIEnv *env() {
        return _env;
    }

    void addRef(jobject obj) {
        _refs.push_back(obj);
    }

    bool catchAndHandleException() {
        if (_env != nullptr && _env->ExceptionCheck()) {
            _env->ExceptionClear();
            return true;
        }
        return false;
    }

private:
    bool _needsDetach = false;
    JNIEnv *_env = nullptr;
    std::vector<jobject> _refs;
};

int random_buffer(uint8_t *buf, size_t len) {
    // Check whether the JVM instance has been set at `JNI_OnLoad`.
    // https://github.com/trustwallet/wallet-core/pull/3984
    if (cachedJVM == nullptr) {
        std::ifstream randomData("/dev/urandom", std::ios::in | std::ios::binary);
        if (!randomData.is_open()) {
            return -1;
        }

        randomData.read(reinterpret_cast<char*>(buf), static_cast<std::streamsize>(len));

        // Check if read was successful before closing
        bool readSuccess = randomData.good() && randomData.gcount() == static_cast<std::streamsize>(len);
        randomData.close();

        if (!readSuccess) {
            return -1;
        }

        return 0;
    }

    JNIEnvContext context;
    if (!context.ok()) {
        return -1;
    }

    // SecureRandom random = new SecureRandom();
    jclass secureRandomClass = context.env()->FindClass("java/security/SecureRandom");
    context.addRef(secureRandomClass);
    if (context.catchAndHandleException() || secureRandomClass == nullptr) {
        return -1;
    }

    // `jbyte*` is not a JNI reference. Do not put it into `context.addRef()`.
    jmethodID constructor = context.env()->GetMethodID(secureRandomClass, "<init>", "()V");
    if (context.catchAndHandleException() || constructor == nullptr) {
        return -1;
    }

    jobject random = context.env()->NewObject(secureRandomClass, constructor);
    context.addRef(random);
    if (context.catchAndHandleException() || random == nullptr) {
        return -1;
    }

    // byte array[] = new byte[len];
    jbyteArray array = context.env()->NewByteArray(static_cast<jsize>(len));
    context.addRef(array);
    if (context.catchAndHandleException() || array == nullptr) {
        return -1;
    }

    // random.nextBytes(bytes);
    // `jbyte*` is not a JNI reference. Do not put it into `context.addRef()`.
    jmethodID nextBytes = context.env()->GetMethodID(secureRandomClass, "nextBytes", "([B)V");
    if (context.catchAndHandleException() || nextBytes == nullptr) {
        return -1;
    }

    context.env()->CallVoidMethod(random, nextBytes, array);
    if (context.catchAndHandleException()) {
        return -1;
    }

    // `jbyte*` is not a JNI reference. Do not put it into `context.addRef()`.
    jbyte* bytes = context.env()->GetByteArrayElements(array, nullptr);
    if (context.catchAndHandleException() || bytes == nullptr) {
        return -1;
    }

    memcpy(buf, bytes, len);
    context.env()->ReleaseByteArrayElements(array, bytes, JNI_ABORT);

    return 0;
}
