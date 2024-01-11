// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <jni.h>
#include "TWJNI.h"

TW_EXTERN_C_BEGIN

/// Converts a TWData (will be deleted within this call) to a Java byte array.
jbyteArray TWDataJByteArray(TWData *data, JNIEnv *env);

/// Converts a Java byte array to a TWData, caller must delete it after use.
TWData * TWDataCreateWithJByteArray(JNIEnv *env, jbyteArray array);

TW_EXTERN_C_END
