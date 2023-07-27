#include <TrustWalletCore/TWAnySignerTangem.h>
#include "TWJNI.h"
#include "AnySigner.h"

static JavaVM* cachedJVM = nullptr;

TWData *_Nonnull signerCallback(jobject signer, TWData *_Nonnull data) {
    JNIEnv *env = nullptr;
    cachedJVM->AttachCurrentThread(&env, nullptr);

    jbyteArray argument = TWDataJByteArray(data, env);
    jclass signerClazz = env->GetObjectClass(signer);
    jmethodID signMethodId = env->GetMethodID(signerClazz, "sign", "([B)[B");
    jbyteArray signResult = (jbyteArray) env->CallObjectMethod(signer, signMethodId, argument);

    TWData *ret = TWDataCreateWithJByteArray(env, signResult);
    cachedJVM->DetachCurrentThread();
    return ret;
}

extern "C" jbyteArray Java_wallet_core_java_AnySigner_nativeSignExternally(JNIEnv *env, jclass thisClass, jbyteArray publicKey, jbyteArray input, jint coin, jobject signer) {
    env->GetJavaVM(&cachedJVM);
    TWData *inputData = TWDataCreateWithJByteArray(env, input);
    TWData *publicKeyData = TWDataCreateWithJByteArray(env, publicKey);

    jobject globalSigner = env->NewGlobalRef(signer);
    auto externalSigner = std::bind(signerCallback, globalSigner, std::placeholders::_1);
    TWData *outputData = TWAnySignerSignExternally(inputData, static_cast<TWCoinType>(coin), publicKeyData, externalSigner);
    jbyteArray resultData = TWDataJByteArray(outputData, env);

    env->DeleteGlobalRef(globalSigner);
    TWDataDelete(inputData);
    TWDataDelete(publicKeyData);
    cachedJVM = nullptr;
    return resultData;
}
