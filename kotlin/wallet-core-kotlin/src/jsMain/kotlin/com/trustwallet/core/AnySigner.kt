// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
