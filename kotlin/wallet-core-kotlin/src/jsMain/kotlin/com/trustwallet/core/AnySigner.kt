// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

import WalletCore

actual object AnySigner {

    actual fun sign(input: ByteArray, coin: CoinType): ByteArray =
        WalletCore.Instance.AnySigner.sign(input.asUInt8Array(), coin.jsValue).asByteArray()

    actual fun supportsJson(coin: CoinType): Boolean =
        WalletCore.Instance.AnySigner.supportsJSON(coin.jsValue)

    actual fun signJson(json: String, key: ByteArray, coin: CoinType): String =
        TODO()

    actual fun plan(input: ByteArray, coin: CoinType): ByteArray =
        WalletCore.Instance.AnySigner.plan(input.asUInt8Array(), coin.jsValue).asByteArray()
}
