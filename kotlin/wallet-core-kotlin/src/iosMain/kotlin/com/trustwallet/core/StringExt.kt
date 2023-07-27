// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.CValuesRef
import kotlinx.cinterop.toKString

internal fun String?.toTwString(): COpaquePointer? =
    this?.let { TWStringCreateWithUTF8Bytes(it) }

internal fun CValuesRef<*>?.fromTwString(): String? =
    this?.let { TWStringUTF8Bytes(it)?.toKString() }
