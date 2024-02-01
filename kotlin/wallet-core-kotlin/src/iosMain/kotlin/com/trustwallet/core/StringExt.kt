// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.CValuesRef
import kotlinx.cinterop.toKString

internal fun String?.toTwString(): COpaquePointer? =
    this?.let { TWStringCreateWithUTF8Bytes(it) }

internal fun CValuesRef<*>?.fromTwString(): String? =
    this?.let { TWStringUTF8Bytes(it)?.toKString() }
