package com.trustwallet.core

import org.khronos.webgl.get

internal typealias UInt8Array = org.khronos.webgl.Uint8Array

internal fun UInt8Array.toByteArray(): ByteArray =
    ByteArray(length, ::get)

internal fun ByteArray.toUInt8Array(): UInt8Array =
    UInt8Array(toTypedArray())
