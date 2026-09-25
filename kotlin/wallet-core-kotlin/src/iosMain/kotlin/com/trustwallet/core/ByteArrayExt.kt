// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.readBytes
import kotlinx.cinterop.toCValues

// Build ByteArray from TWData, and then delete TWData
@OptIn(kotlinx.cinterop.ExperimentalForeignApi::class)
internal fun COpaquePointer?.readTwBytes(): ByteArray? =
    this?.let {
        val result = TWDataBytes(it)?.readBytes(TWDataSize(it).toInt())
        TWDataDelete(it)
        result
    }
