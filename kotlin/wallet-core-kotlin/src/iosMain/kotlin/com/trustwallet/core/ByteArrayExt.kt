// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.readBytes
import kotlinx.cinterop.toCValues

internal fun COpaquePointer?.readTwBytes(): ByteArray? =
    TWDataBytes(this)?.readBytes(TWDataSize(this).toInt())

@OptIn(ExperimentalUnsignedTypes::class)
internal fun ByteArray?.toTwData(): COpaquePointer? =
    TWDataCreateWithBytes(this?.toUByteArray()?.toCValues(), this?.size?.toULong() ?: 0u)
