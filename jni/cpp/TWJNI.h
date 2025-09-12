// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWBase.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWString.h>

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrustWalletCore/TWBitcoinScript.h>
#include <TrustWalletCore/TWSegwitAddress.h>
#include <TrustWalletCore/TWAnySigner.h>

#include <jni.h>
#include "TWJNIData.h"
#include "TWJNIString.h"

#define JNI_CHECK_NULL_AND_RETURN_VOID(env, param, paramName) \
    do { \
        if (param == NULL) { \
            jclass exceptionClass = (*env)->FindClass(env, "java/lang/IllegalArgumentException"); \
            if (exceptionClass != NULL) { \
                (*env)->ThrowNew(env, exceptionClass, paramName " parameter cannot be null"); \
            } \
            return; \
        } \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_ZERO(env, param, paramName) \
    do { \
        if (param == NULL) { \
            jclass exceptionClass = (*env)->FindClass(env, "java/lang/IllegalArgumentException"); \
            if (exceptionClass != NULL) { \
                (*env)->ThrowNew(env, exceptionClass, paramName " parameter cannot be null"); \
            } \
            return 0; \
        } \
    } while(0)

#define JNI_CHECK_NULL_AND_RETURN_NULL(env, param, paramName) \
    do { \
        if (param == NULL) { \
            jclass exceptionClass = (*env)->FindClass(env, "java/lang/IllegalArgumentException"); \
            if (exceptionClass != NULL) { \
                (*env)->ThrowNew(env, exceptionClass, paramName " parameter cannot be null"); \
            } \
            return NULL; \
        } \
    } while(0)
