package com.trustwallet.core

import WalletCore

internal actual fun signImpl(input: ByteArray, coin: CoinType): ByteArray =
    WalletCore.Instance.AnySigner.sign(input.toUInt8Array(), coin._value)
        .unsafeCast<UInt8Array>()
        .toByteArray()

internal actual fun supportsJsonImpl(coin: CoinType): Boolean =
    WalletCore.Instance.AnySigner.supportsJSON(coin._value) as Boolean

internal actual fun signJsonImpl(json: String, key: ByteArray, coin: CoinType): String =
    TODO()

internal actual fun planImpl(input: ByteArray, coin: CoinType): ByteArray =
    WalletCore.Instance.AnySigner.plan(input.toUInt8Array(), coin._value)
        .unsafeCast<UInt8Array>()
        .toByteArray()
