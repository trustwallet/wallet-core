// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.CValuesRef
import kotlinx.cinterop.toKString

internal fun String?.toTwString(): COpaquePointer? =
    this?.let { TWStringCreateWithUTF8Bytes(it) }

// Build String from TWString, and then delete TWString
internal fun CValuesRef<*>?.fromTwString(): String? =
    this?.let {
        val result = TWStringUTF8Bytes(it)?.toKString()
        TWStringDelete(it)
        result
    }
