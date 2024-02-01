// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

package com.trustwallet.core

actual object AnySigner {

    @JvmStatic
    actual external fun sign(input: ByteArray, coin: CoinType): ByteArray

    @JvmStatic
    actual external fun supportsJson(coin: CoinType): Boolean

    @JvmStatic
    actual external fun signJson(json: String, key: ByteArray, coin: CoinType): String

    @JvmStatic
    actual external fun plan(input: ByteArray, coin: CoinType): ByteArray
}
