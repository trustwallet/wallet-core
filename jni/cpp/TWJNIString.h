// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <jni.h>
#include "TWJNI.h"

TW_EXTERN_C_BEGIN

/// Converts a TWString (will be deleted within this call) to a Java string.
jstring TWStringJString(TWString *string, JNIEnv *env);

/// Converts a Java string to a TWString, caller must delete it after use.
TWString * TWStringCreateWithJString(JNIEnv *env, jstring string);

TW_EXTERN_C_END
