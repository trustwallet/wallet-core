package com.trustwallet.core

import kotlinx.cinterop.COpaquePointer
import kotlinx.cinterop.CValuesRef
import kotlinx.cinterop.toKString

internal fun String?.toTwString(): COpaquePointer? =
    TWStringCreateWithUTF8Bytes(this)

internal fun CValuesRef<*>?.fromTwString(): String? =
    TWStringUTF8Bytes(this)?.toKString()
