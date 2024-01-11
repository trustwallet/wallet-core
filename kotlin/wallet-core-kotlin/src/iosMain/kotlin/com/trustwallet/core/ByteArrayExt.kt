// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.readBytes
import kotlinx.cinterop.toCValues

internal fun COpaquePointer?.readTwBytes(): ByteArray? =
    TWDataBytes(this)?.readBytes(TWDataSize(this).toInt())

@OptIn(ExperimentalUnsignedTypes::class)
internal fun ByteArray?.toTwData(): COpaquePointer? =
    TWDataCreateWithBytes(this?.toUByteArray()?.toCValues(), this?.size?.toULong() ?: 0u)
