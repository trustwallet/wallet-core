// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import kotlinx.cinterop.CValuesRef
import kotlinx.cinterop.toKString

// Build String from TWString, and then delete TWString
@OptIn(kotlinx.cinterop.ExperimentalForeignApi::class)
internal fun CValuesRef<*>?.fromTwString(): String? =
    this?.let {
        val result = TWStringUTF8Bytes(it)?.toKString()
        TWStringDelete(it)
        result
    }
