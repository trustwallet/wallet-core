// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
