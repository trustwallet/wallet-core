// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

package com.trustwallet.core

import org.khronos.webgl.get

internal typealias UInt8Array = org.khronos.webgl.Uint8Array

internal fun UInt8Array.toByteArray(): ByteArray =
    ByteArray(length, ::get)

internal fun ByteArray.toUInt8Array(): UInt8Array =
    UInt8Array(toTypedArray())
